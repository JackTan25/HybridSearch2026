import json
import struct
import numpy as np

def load_dense(dense_save_file: str):
    dense_embeddings = []
    with open(dense_save_file, 'rb') as f:
        while True:
            # 读取向量的维度（4字节整数，小端格式）
            dim_data = f.read(4)
            if not dim_data:  # 文件结束
                break
            dim = struct.unpack('<i', dim_data)[0]
            
            # 读取对应维度的浮点数向量
            vector_data = f.read(dim * 2)  # 每个浮点数4字节
            one_dense = np.frombuffer(vector_data, dtype=np.float16)
            
            dense_embeddings.append(one_dense)
    return np.array(dense_embeddings)

def load_sparse(sparse_save_file: str):
    with open(sparse_save_file, 'r', encoding='utf-8') as file:
        return json.load(file)

def load_result(dense_save_file: str):
    dense_embeddings = []
    with open(dense_save_file, 'rb') as f:
        while True:
            # 读取向量的维度（4字节整数，小端格式）
            dim_data = f.read(4)
            if not dim_data:  # 文件结束
                break
            dim = struct.unpack('<i', dim_data)[0]
            
            # 读取对应维度的浮点数向量
            vector_data = f.read(dim * 2)  # 每个浮点数4字节
            one_dense = np.frombuffer(vector_data, dtype=np.float32)
            
            dense_embeddings.append(one_dense)
    return np.array(dense_embeddings)

# # 示例用法
# dense_save_file = "/home/ubuntu/experiments/small_embedding/CQADupStack_en/dense_embeddings/vectors/CQADupStack_en_dense1.fvecs"
# dense_embeddings = load_result(dense_save_file)

# print(f"Loaded {len(dense_embeddings)} embeddings with dimension {dense_embeddings[0]}")
