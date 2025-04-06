import pandas as pd

# 创建示例 DataFrame
data = {
    'query-id': [1, 2, 1, 2, 1],
    'value': [10, 20, 30, 40, 50]
}
df = pd.DataFrame(data)

# 按 'query-id' 分组
grouped = df.groupby('query-id')
print(grouped)
# 遍历每个组
for query_id, group in grouped:
    print(f"Query ID: {query_id}")
    print(group)
    print("-" * 20)