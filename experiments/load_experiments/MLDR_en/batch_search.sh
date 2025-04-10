# ## tensor index
# python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/build/tensor_index.py

## search
### single_search
python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search/single_road/dense_search.py
sleep 10
python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search/single_road/sparse_search.py
sleep 10
python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search/single_road/tensor_search.py ## 8192
sleep 10
python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search/single_road/fulltext_search.py 
sleep 10
### two_search
python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search/two_roads/dense_fulltext_search.py
sleep 10
python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search/two_roads/dense_tensor_search.py
sleep 10
python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search/two_roads/fulltext_tensor_search.py
sleep 10
python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search/two_roads/sparse_dense_search.py
sleep 10
python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search/two_roads/sparse_fulltext_search.py
sleep 10
python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search/two_roads/sparse_tensor_search.py
sleep 10
### three search
python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search/three_roads/dense_fulltext_tensor.py
sleep 10
python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search/three_roads/sparse_dense_fulltext_search.py
sleep 10
python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search/three_roads/sparse_dense_tensor_search.py
sleep 10
python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search/three_roads/sparse_fulltext_tensor_search.py
sleep 10
### four search
python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search/four_roads/sparse_fulltext_tensor_dense_search.py
sleep 10
## search100
### two_search
python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search100/two_roads/dense_fulltext_search.py
sleep 10
python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search100/two_roads/dense_tensor_search.py
sleep 10
python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search100/two_roads/fulltext_tensor_search.py
sleep 10
python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search100/two_roads/sparse_dense_search.py
sleep 10
python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search100/two_roads/sparse_fulltext_search.py
sleep 10
python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search100/two_roads/sparse_tensor_search.py
sleep 10
### three search
python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search100/three_roads/dense_fulltext_tensor.py
sleep 10
python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search100/three_roads/sparse_dense_fulltext_search.py
sleep 10
python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search100/three_roads/sparse_dense_tensor_search.py
sleep 10
python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search100/three_roads/sparse_fulltext_tensor_search.py
sleep 10
### four search
python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search100/four_roads/sparse_fulltext_tensor_dense_search.py
sleep 10
## search1000
### two_search
python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search1000/two_roads/dense_fulltext_search.py
sleep 10
python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search1000/two_roads/dense_tensor_search.py
sleep 10
python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search1000/two_roads/fulltext_tensor_search.py
sleep 10
python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search1000/two_roads/sparse_dense_search.py
sleep 10
python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search1000/two_roads/sparse_fulltext_search.py
sleep 10
python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search1000/two_roads/sparse_tensor_search.py
sleep 10
### three search
python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search1000/three_roads/dense_fulltext_tensor.
sleep 10
python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search1000/three_roads/sparse_dense_fulltext_search.py
sleep 10
python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search1000/three_roads/sparse_dense_tensor_search.py
sleep 10
python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search1000/three_roads/sparse_fulltext_tensor_search.py
sleep 10
### four search
python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search1000/four_roads/sparse_fulltext_tensor_dense_search.py
# ## search_tensor_rank
# ### two_search
# python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search_tensor_rank/two_roads/dense_fulltext_search.py
# python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search_tensor_rank/two_roads/dense_tensor_search.py
# python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search_tensor_rank/two_roads/fulltext_tensor_search.py
# python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search_tensor_rank/two_roads/sparse_dense_search.py
# python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search_tensor_rank/two_roads/sparse_fulltext_search.py
# python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search_tensor_rank/two_roads/sparse_tensor_search.py
# ### three search
# python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search_tensor_rank/three_roads/dense_fulltext_tensor.py
# python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search_tensor_rank/three_roads/sparse_dense_fulltext_search.py
# python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search_tensor_rank/three_roads/sparse_dense_tensor_search.py
# python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search_tensor_rank/three_roads/sparse_fulltext_tensor_search.py

# ## search_tensor_rank100
# ### two_search
# python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search_tensor_rank100/two_roads/dense_fulltext_search.py
# python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search_tensor_rank100/two_roads/sparse_dense_search.py
# python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search_tensor_rank100/two_roads/sparse_fulltext_search.py
# ### three search
# python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search_tensor_rank100/three_roads/sparse_dense_fulltext_search.py

# ## search_tensor_rank1000
# ### two_search
# python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search_tensor_rank1000/two_roads/dense_fulltext_search.py
# python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search_tensor_rank1000/two_roads/sparse_dense_search.py
# python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search_tensor_rank1000/two_roads/sparse_fulltext_search.py
# ### three search
# python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search_tensor_rank1000/three_roads/sparse_dense_fulltext_search.py

# ## search_weighted_sum
# ### two_search
# python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search_weigthed_sum/two_roads/dense_fulltext_search.py
# python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search_weigthed_sum/two_roads/sparse_dense_search.py
# python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search_weigthed_sum/two_roads/sparse_fulltext_search.py
# ### three search
# python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search_weigthed_sum/three_roads/sparse_dense_fulltext_search.py

# ## search_weighted_sum100
# ### two_search
# python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search_weigthed_sum100/two_roads/dense_fulltext_search.py
# python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search_weigthed_sum100/two_roads/sparse_dense_search.py
# python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search_weigthed_sum100/two_roads/sparse_fulltext_search.py
# ### three search
# python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search_weigthed_sum100/three_roads/sparse_dense_fulltext_search.py

# ## search_weighted_sum1000
# ### two_search
# python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search_weigthed_sum1000/two_roads/dense_fulltext_search.py
# python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search_weigthed_sum1000/two_roads/sparse_dense_search.py
# python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search_weigthed_sum1000/two_roads/sparse_fulltext_search.py
# ### three search
# python3 /home/ubuntu/hybridsearch/experiments/load_experiments/MLDR_en/search_weigthed_sum1000/three_roads/sparse_dense_fulltext_search.py
