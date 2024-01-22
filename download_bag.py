# import requests
# import sys
# import os
# bag_name = sys.argv[1]
# start_index = (int)(sys.argv[2])
# end_index = (int)(sys.argv[3])
# for idx in range(start_index, end_index+1):
#     db_file_name = bag_name + "_" + str(idx) + ".db"
#     if os.path.exists(db_file_name):
#         continue
#     url = "https://bagdb.pluscn.cn/api/v1/bags"
#     bag_name_specific = bag_name + "_" + str(idx) + ".bag"
#     query_params = {'bag_name': bag_name_specific}
#     response = requests.get(url, params=query_params)
#     if response.status_code == 200:
#         data = response.json()
#     link = data[0]["fastbag_path"].split("?")[0]
#     print(link)
#     response = requests.get(link, stream=True)
#     if response.status_code == 200:
#         with open(db_file_name, 'wb') as file:
#             for chunk in response.iter_content(chunk_size=8192):
#                 file.write(chunk)
#         print("File downloaded successfully as", db_file_name)
#     else:
#         print("File download failed with status code:", response.status_code)
import requests
import sys
import os
bag_name = sys.argv[1]
start_index = (int)(sys.argv[2])
end_index = (int)(sys.argv[3])
for idx in range(start_index, end_index+1):
    db_file_name = bag_name + "_" + str(idx) + ".db"
    if os.path.exists(db_file_name):
        os.remove(db_file_name)
    url = "https://bagdb.pluscn.cn/api/v1/bags"
    bag_name_specific = bag_name + "_" + str(idx) + ".bag"
    query_params = {'bag_name': bag_name_specific}
    response = requests.get(url, params=query_params)
    if response.status_code == 200:
        data = response.json()
    link = data[0]["fastbag_path"].split("?")[0]
    print(link)
    response = requests.get(link, stream=True)
    if response.status_code == 200:
        with open(db_file_name, 'wb') as file:
            for chunk in response.iter_content(chunk_size=1024*1024*10):
                file.write(chunk)
        print("File downloaded successfully as", db_file_name)
    else:
        print("File download failed with status code:", response.status_code)

# import requests
# import sys
# bag_name = sys.argv[1]

# url = "https://bagdb.pluscn.cn/api/v1/bags"
# bag_name_specific = bag_name + ".bag"
# print("bag_name_specific = ", bag_name_specific)
# query_params = {'bag_name': bag_name_specific}
# response = requests.get(url, params=query_params)
# if response.status_code == 200:
#     data = response.json()
#     print(data)
# link = data[0]["fastbag_path"].split("?")[0]
# print(link)
# response = requests.get(link, stream=True)
# if response.status_code == 200:
#     with open(bag_name_specific, 'wb') as file:
#         for chunk in response.iter_content(chunk_size=8192):
#             file.write(chunk)
#     print("File downloaded successfully as", bag_name_specific)
# else:
#     print("File download failed with status code:", response.status_code)
