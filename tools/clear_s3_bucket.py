from minio import Minio

client = Minio(
    endpoint = "127.0.0.1:9000",
    access_key="minioadmin",
    secret_key="minioadmin",
    secure = False,
)


objects = client.list_objects("hybridsearch", recursive=True)
for obj in objects:
    print(obj.object_name)
    client.remove_object("hybridsearch", obj.object_name)
