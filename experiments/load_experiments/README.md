This Repo is used to test [HybridSearch2026](https://github.com/JackTan25/HybridSearch2026) perfmormance on different datasets as below.
## Datasets
| Dataset       | Domain          | Task                          | #Corpus   | #Query | Avg. Length of Docs | Full-Text | Sparse   | Dense   | Tensor |
|---------------|-----------------|-------------------------------|-----------|--------|---------------------|-----------|----------|---------|--------|
| MSMA(en) | Miscellaneous   | Passage Retrieval             | 8,841,823 | 43     | 56                 | 2.9G      | 13G      | 36G     | -      |
| DBPE(en)  | Wikipedia       | Entity Retrieval              | 4,635,922 | 400    | 50                 | 1.4G      | 6.9G     | 18.2G   | 58G    |
| MCCN(zh)  | News            | Question Answer               | 935,162   | 339    | 1,263              | 2.3G      | 6.9G     | 3.8G    | 148G   |
| TOUC(en)  | Miscellaneous   | Instrument Retrieval           | 382,545   | 49     | 292                | 184M      | 1.6G     | 1.5G    | 56G    |
| MLDR(zh)  | Wiki,K.,Wada    | Long-Document Retrieval        | 200,000   | 800    | 4,249              | 3.1G      | 5.2G     | 791M    | 186G   |
| MLDR(en)  | Wikipedia       | Long-Document Retrieval        | 200,000   | 800    | 3,308              | 3.1G      | 4.4G     | 791M    | 94G    |
| TRE(C)   | Bio-Medical     | Bio-Medical Information Retrieval | 171,332 | 50     | 161                | 184M      | 554M     | 688M    | 16G    |
| TQA(en)    | -               | -                              | -         | -      | -                  | -         | -        | -       | -      |

*The last four columns denote the corpus size for each data type.*

## Experiments
We check NDCG@10, memory usage, latency as our metrics. In each dircetory, We have build and search subdirectories. The build directory is used to build the index and load data, and the search directory is used to search the index. The build directory contains the following files:
- bmp_index.py: build BMP index
- data_insert.py: insert data into framework
- dense_index.py: build dense index
- tensor_index.py: build tensor index
- fulltext_index.py: build full-text index
- vec_read.py: read dense vectors and sparse data
- colbert_read.py: read tensor data

The search directory contains the following subdirectories:
- single_road: single road search
- two_road: two road search
- three_road: three road search
- four_road: four road search

we can try command like below to run the experiments:
```bash
cd Dataset_Directory
bash batch_search.sh
```
For more detais, please refer to the scripts in each directory.