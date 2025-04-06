import struct
import numpy as np
from tqdm import tqdm

def load_colbert_list(multivec_save_file: str):
    multivec_embeddings = []
    with open(multivec_save_file, 'rb') as f:
        while True:
            # 读取嵌入数量（l）
            l_bytes = f.read(4)
            if not l_bytes:
                break  # 文件结束
            l = struct.unpack('<i', l_bytes)[0]

            # 存储当前嵌入的所有向量
            one_multivec = []
            for _ in range(l):
                # 读取向量维度（dim）
                dim = struct.unpack('<i', f.read(4))[0]

                # 读取 dim 个 float16 的值
                vec = np.fromfile(f, dtype=np.float16, count=dim)
                one_multivec.append(vec)

            multivec_embeddings.append(np.array(one_multivec))

    return multivec_embeddings

# # 示例用法
# multivec_save_file = '/home/ubuntu/data_download_data/embedding_reserve/mldr_en/tensor_embeddings/vectors/MLDR_en_bert1.fvecs'
# multivec_embeddings = load_colbert_list(multivec_save_file)

# # 检查加载结果
# print(f"Loaded {len(multivec_embeddings)} embeddings.")
# for i, multivec in enumerate(multivec_embeddings[:3]):  # 打印前3个嵌入
#     print(f"Embedding {i}: shape = {multivec.shape}")
