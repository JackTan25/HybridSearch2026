# A Deep Dive into Advanced Hybrid Search Architectures: [Experiments & Analysis]

## Overview

The code repository provides the implementation for the paper *"A Deep Dive into Advanced Hybrid Search Architectures"*. This study systematically evaluates the performance of advanced hybrid search architectures by integrating four retrieval paradigms—full-text search (FTS), sparse vector search (SVS), dense vector search (DVS), and tensor search (TenS)—along with multiple re-ranking strategies.

## Datasets

| Dataset  | Website                                                                   | Download                                                                                        | Domain           | Task                              | #Corpus   | #Query | Avg. Length of Docs | Full-Text | Sparse | Dense  | Tensor |
| -------- | ------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------- | ---------------- | --------------------------------- | --------- | ------ | ------------------- | --------- | ------ | ------ | ------ |
| MSMA(en) | [Homepage](https://microsoft.github.io/msmarco/)                          | [Link](https://public.ukp.informatik.tu-darmstadt.de/thakur/BEIR/datasets/msmarco.zip)          | Miscellaneous    | Passage Retrieval                 | 8,841,823 | 43     | 56                  | 2.9GB     | 13GB   | 36GB   | 254GB  |
| DBPE(en) | [Homepage](https://github.com/iai-group/DBpedia-Entity/)                  | [Link](https://public.ukp.informatik.tu-darmstadt.de/thakur/BEIR/datasets/dbpedia-entity.zip)   | Wikipedia        | Entity Retrieval                  | 4,635,922 | 400    | 50                  | 1.4GB     | 6.9GB  | 18.2GB | 58GB   |
| MCCN(zh) | [Homepage](https://huggingface.co/datasets/intfloat/multilingual_cc_news) | [Link](https://huggingface.co/datasets/intfloat/multilingual_cc_news/viewer/zh)                 | News             | Question Answering                | 935,162   | 339    | 1,263               | 2.3GB     | 6.9GB  | 3.8GB  | 148GB  |
| TOUC(en) | [Homepage](https://webis.de/events/touche-20/shared-task-1.html)          | [Link](https://public.ukp.informatik.tu-darmstadt.de/thakur/BEIR/datasets/webis-touche2020.zip) | Miscellaneous    | Argument Retrieval                | 382,545   | 49     | 292                 | 184MB     | 1.6GB  | 1.5GB  | 56GB   |
| MLDR(zh) | [Homepage](https://huggingface.co/datasets/Shitao/MLDR)                   | [Link](https://huggingface.co/datasets/Shitao/MLDR/tree/main/mldr-v1.0-zh)                      | Wikipedia, Wudao | Long-Document Retrieval           | 200,000   | 800    | 4,249               | 3.1GB     | 5.2GB  | 791MB  | 186GB  |
| MLDR(en) | [Homepage](https://huggingface.co/datasets/Shitao/MLDR)                   | [Link](https://huggingface.co/datasets/Shitao/MLDR/tree/main/mldr-v1.0-en)                      | Wikipedia        | Long-Document Retrieval           | 200,000   | 800    | 3,308               | 3.1GB     | 4.4GB  | 791MB  | 94GB   |
| TREC(en) | [Homepage](https://ir.nist.gov/covidSubmit/index.html)                    | [Link](https://public.ukp.informatik.tu-darmstadt.de/thakur/BEIR/datasets/trec-covid.zip)       | Bio-Medical      | Bio-Medical Information Retrieval | 171,332   | 50     | 161                 | 184MB     | 554MB  | 688MB  | 16GB   |
| FIQA(en) | [Homepage](https://sites.google.com/view/fiqa/)                           | [Link](https://public.ukp.informatik.tu-darmstadt.de/thakur/BEIR/datasets/fiqa.zip)             | Finance          | Question Answering                | 57,638    | 648    | 132                 | 43MB      | 137MB  | 232MB  | 4GB    |
| CQAD(en) | [Homepage](http://nlp.cis.unimelb.edu.au/resources/cqadupstack/)          | [Link](https://public.ukp.informatik.tu-darmstadt.de/thakur/BEIR/datasets/cqadupstack.zip)      | StackExchange    | Duplicate-Question Retrieval      | 40,221    | 1,570  | 129                 | 19MB      | 63MB   | 164MB  | 1GB    |
| SCID(en) | [Homepage](https://allenai.org/data/scidocs)                              | [Link](https://public.ukp.informatik.tu-darmstadt.de/thakur/BEIR/datasets/scidocs.zip)          | Scientific       | Citation Prediction               | 25,657    | 1,000  | 176                 | 30MB      | 89MB   | 106MB  | 2.4GB  |
| SCIF(en) | [Homepage](https://github.com/allenai/scifact)                            | [Link](https://public.ukp.informatik.tu-darmstadt.de/thakur/BEIR/datasets/scifact.zip)          | Scientific       | Fact Checking                     | 5,183     | 809    | 214                 | 7.5MB     | 23MB   | 24MB   | 676MB  |

Note: The last four columns denote the corpus size for each data type.

## Usage

**<u>Data Loading and Index Building</u>**: In the `build` directory, use the following files for corresponding operations:

- `data_insert.py`: Inserts data into the evaluation framework.
- `bmp_index.py`: Builds the Block-Max Pruning (BMP) index for sparse vector search (SVS).
- `dense_index.py`: Builds the dense vector index for dense vector search (DVS).
- `tensor_index.py`: Builds the tensor index for tensor search (TenS).
- `fulltext_index.py`: Builds the full - text index for full-text search (FTS).

**<u>Retrieval Experiments</u>**: In the `search` directory, conduct experiments according to different retrieval path combinations:

- `single_road`: Single-path search, evaluating the performance of FTS, SVS, DVS, and TenS used independently.
- `two_road`: Two-path search, such as performance tests of combinations like FTS + DVS, FTS + SVS.
- `three_road`: Three-path search, e.g., experiments on combinations like FTS + DVS + SVS.
- `four_road`: Four-path search, evaluating the performance of the FTS + DVS + SVS + TenS combination.

**<u>Running Experiment Commands</u>**: Enter the directory corresponding to the dataset ([Dataset_Directory](experiments/load_experiments)) and execute the command bash batch_search.sh to run the corresponding retrieval experiments. For detailed experimental settings and script functions, refer to the script files in each directory.

## Results

For the experimental results and analyses, please refer to the main text. Additional supplementary results are provided in this section.

## Citation and Contribution

**<u>Citation</u>**: If you use the code or experimental results from this repository, please cite the paper "A Deep Dive into Advanced Hybrid Search Architectures".

**<u>Contribution</u>**: We welcome researchers and developers interested in this project to contribute code, raise issues, or provide suggestions. Please follow the open-source community's norms, submit code changes by creating a Pull Request, and describe the changes and their purposes in detail when submitting.

## License

This project is licensed under the Apache 2.0 license.
