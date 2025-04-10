# This file shows how cluster in hybridsearch work.
# To run this script, you should first run these 3 commands to start 3 nodes seperately,
# you can run these commands at the root directory of project after building the code:

# ./cmake-build-debug/src/hybridsearch --config conf/leader.toml
# ./cmake-build-debug/src/hybridsearch --config conf/follower.toml
# ./cmake-build-debug/src/hybridsearch --config conf/learner.toml

echo '\n\n--show admin configs'
curl --request GET \
    --url http://localhost:23822/admin/configs \
    --header 'accept: application/json'

echo '\n\n--show admin variables'
curl --request GET \
    --url http://localhost:23822/admin/variables \
    --header 'accept: application/json'

echo '\n\n--show admin catalogs'
curl --request GET \
    --url http://localhost:23820/admin/catalogs \
    --header 'accept: application/json'

echo '\n\n--show admin wal'
curl --request GET \
    --url http://localhost:23822/admin/logs \
    --header 'accept: application/json'

# setting leader
echo '--setting leader'
curl --request POST \
    --url http://localhost:23821/admin/node/current \
    --header 'accept: application/json' \
    --header 'content-type: application/json' \
    --data ' 
    {
        "role" : "leader",
        "name" : "boss"
    } '

# setting follower and register leader
echo '\n\n--setting follower and register leader'
curl --request POST \
    --url http://localhost:23822/admin/node/current \
    --header 'accept: application/json' \
    --header 'content-type: application/json' \
    --data ' 
    {
        "role" : "follower",
        "name" : "following",
        "address" : "0.0.0.0:23851"
    } '

# setting learner and register leader
echo '\n\n--setting learner and register leader'
curl --request POST \
    --url http://localhost:23823/admin/node/current \
    --header 'accept: application/json' \
    --header 'content-type: application/json' \
    --data ' 
    {
        "role" : "learner",
        "name" : "learning",
        "address" : "0.0.0.0:23851"
    } '

# check each nodes
echo '\n\n--check leader node'
curl --request GET \
    --url http://localhost:23821/admin/node/current \
    --header 'accept: application/json'

echo '\n\n--check follower node'
curl --request GET \
    --url http://localhost:23822/admin/node/current \
    --header 'accept: application/json'

echo '\n\n--check learner node'
curl --request GET \
    --url http://localhost:23823/admin/node/current \
    --header 'accept: application/json'

echo '\n\n--on leader, check the learner node in cluster'
curl --request GET \
    --url http://localhost:23821/admin/node/learning \
    --header 'accept: application/json'

echo '\n\n--on follower, check the leader in cluster'
curl --request GET \
    --url http://localhost:23822/admin/node/boss \
    --header 'accept: application/json'

echo '\n\n--on learner, list all nodes in cluster'
curl --request GET \
    --url http://localhost:23822/admin/nodes \
    --header 'accept: application/json'

