#include "NvInfer.h"
#include <cuda.h>
#include <cuda_runtime_api.h>
#include <unistd.h>
#include <NvOnnxParser.h>
#include <iostream>
#include <memory>
#include <assert.h>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cmath>
using namespace std;
using namespace nvonnxparser;
using namespace nvinfer1;

bool fileExists(const std::string& filePath) {
    return (access(filePath.c_str(), F_OK) != -1);
}


struct InferDeleter
{
    template <typename T>
    void operator()(T* obj) const
    {
        delete obj;
    }
};

template <typename T>
using SampleUniquePtr = std::unique_ptr<T, InferDeleter>;


class Logger : public ILogger           
{
    void log(Severity severity, const char* msg) noexcept override
    {
        // suppress info-level messages
        if (severity <= Severity::kWARNING)
            std::cout << msg << std::endl;
    }
} logger;


bool constructNetwork(SampleUniquePtr<nvinfer1::IBuilder>& builder, SampleUniquePtr<nvinfer1::INetworkDefinition>& network,
                      SampleUniquePtr<nvinfer1::IBuilderConfig>& config, SampleUniquePtr<nvonnxparser::IParser>& parser,
                      std::string modelFile = "../anonymous.onnx", bool fp16 = false)
{
    auto parsed = parser->parseFromFile(modelFile.c_str(), static_cast<int32_t>(ILogger::Severity::kWARNING));
    if (!parsed)
    {
        return false;
    }

    if (fp16)
    {
        config->setFlag(BuilderFlag::kFP16);
    }

    return true;
}

static auto StreamDeleter = [](cudaStream_t* pStream)
{
    if (pStream)
    {
        cudaStreamDestroy(*pStream);
        delete pStream;
    }
};

inline std::unique_ptr<cudaStream_t, decltype(StreamDeleter)> makeCudaStream()
{
    std::unique_ptr<cudaStream_t, decltype(StreamDeleter)> pStream(new cudaStream_t, StreamDeleter);
    if (cudaStreamCreateWithFlags(pStream.get(), cudaStreamNonBlocking) != cudaSuccess)
    {
        pStream.reset(nullptr);
    }

    return pStream;
}


inline void readPGMFile(const std::string& fileName, uint8_t* buffer, int inH, int inW)
{
    std::cout<<"read file: "<<fileName<<endl;
    std::ifstream infile(fileName, std::ifstream::binary);
    assert(infile.is_open() && "Attempting to read from a file that is not open.");
    std::string magic, w, h, max;
    infile >> magic >> w >> h >> max;
    infile.seekg(1, infile.cur);
    infile.read(reinterpret_cast<char*>(buffer), inH * inW);
    for (int i = 0; i < inH * inW; i++)
    {
        std::cout << (" .:-=+*#%@"[buffer[i] / 26]) << (((i + 1) % inW) ? "" : "\n");
    }
}


bool verifyOutput(float* output, const size_t out_size, int gt)
{
    float val{0.0F};
    int idx{0};

    // Calculate Softmax
    float sum{0.0F};
    for (int i = 0; i < out_size; i++)
    {
        output[i] = exp(output[i]);
        sum += output[i];
    }

    std::cout << "Output:" << std::endl;
    for (int i = 0; i < out_size; i++)
    {
        output[i] /= sum;
        val = std::max(val, output[i]);
        if (val == output[i])
        {
            idx = i;
        }

        std::cout << " Prob " << i << "  " << std::fixed << std::setw(5) << std::setprecision(4) << output[i]
                         << " "
                         << "Class " << i << ": " << std::string(int(std::floor(output[i] * 10 + 0.5F)), '*')
                         << std::endl;
    }
    std::cout << std::endl;

    return idx == gt && val > 0.9F;
}

// 加载模型文件
std::vector<unsigned char> load_engine_file(const std::string &file_name)
{
    std::vector<unsigned char> engine_data;
    std::ifstream engine_file(file_name, std::ios::binary);
    assert(engine_file.is_open() && "Unable to load engine file.");
    engine_file.seekg(0, engine_file.end);
    int length = engine_file.tellg();
    engine_data.resize(length);
    engine_file.seekg(0, engine_file.beg);
    engine_file.read(reinterpret_cast<char *>(engine_data.data()), length);
    return engine_data;
}


int main() {

    const string engine_file = "./minist.engin";
    auto plan = load_engine_file(engine_file);
    auto mRuntime = std::shared_ptr<nvinfer1::IRuntime>(createInferRuntime(logger));
    if (!mRuntime)
    {
        cout << "createInferRuntime failed!" << endl;
    }
    auto mEngine = std::shared_ptr<nvinfer1::ICudaEngine>(mRuntime->deserializeCudaEngine(plan.data(), plan.size()));
    if (!mEngine)
    {
        cout << "deserializeCudaEngine failed!" << endl;
    }
    auto context = SampleUniquePtr<nvinfer1::IExecutionContext>(mEngine->createExecutionContext());
    if (!context)
    {
        cout << "createExecutionContext failed!" << endl;
    }
    int inputIndex = mEngine->getBindingIndex("Input3");
    int outputIndex = mEngine->getBindingIndex("Plus214_Output_0");
    cout << inputIndex << " " << outputIndex << endl;
    void* buffers[2]; // Engine requires exactly IEngine::getNbBindings() number of buffers.
    // Create GPU buffers on device -- allocate memory for input and output
    const size_t input_size = 1*1*28*28;
    const size_t output_size = 1*10;
    const size_t batch_size = 1;
    cudaMalloc(&buffers[inputIndex],  input_size * sizeof(float));
    cudaMalloc(&buffers[outputIndex], output_size * sizeof(float));

    // copy input from host (CPU) to device (GPU)  in stream
    float* input  = (float*) malloc(input_size * sizeof(float));
    float* output = (float*) malloc(output_size * sizeof(float));
    std::vector<uint8_t> fileData(input_size);
    int gt = 1;
    readPGMFile(std::to_string(1) + ".pgm", fileData.data(), 28, 28);
    for (int i = 0; i < input_size; i++)
    {
        input[i] = 1.0 - float(fileData[i] / 255.0);
    }
    cudaMemcpy(buffers[inputIndex], input, input_size * sizeof(float), cudaMemcpyHostToDevice);
    // execute inference using context provided by engine
    context->enqueue(batch_size, buffers, 0, nullptr);
    cudaMemcpy(output, buffers[outputIndex], output_size * sizeof(float), cudaMemcpyDeviceToHost);
    if(verifyOutput(output, output_size, gt)) {
        cout << "Engine Answer is correct !" << endl;
    } else {
        cout << "Engine Answer is wrong !" << endl;
    }
    cudaFree(buffers[inputIndex]);
    cudaFree(buffers[outputIndex]);
    free(input);
    free(output);
    return 0;
}