def call(String path) {
    File file = new File(path)
    return file.parentFile.absolutePath;
}
