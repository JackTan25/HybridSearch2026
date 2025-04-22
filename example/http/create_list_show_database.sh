

# create database 'db1'
echo -e '-- create db1'
curl --request POST \
     --url http://localhost:23820/databases/db1 \
     --header 'accept: application/json' \
     --header 'content-type: application/json' \
     --data ' {"create_option": "ignore_if_exists"} '

# create database 'db2'
echo -e '\n-- create db2'
curl --request POST \
     --url http://localhost:23820/databases/db2 \
     --header 'accept: application/json' \
     --header 'content-type: application/json' \
     --data ' {"create_option": "ignore_if_exists"} '


# list databases, there are 'default_db', 'db1' and 'db2' at least.
echo -e '\n-- list database, default_db, db1 and db2 should be included'
curl --request GET \
     --url http://localhost:23820/databases \
     --header 'accept: application/json'

# show each database
echo -e '\n-- show default_db'
curl --request GET \
     --url http://localhost:23820/databases/default_db \
     --header 'accept: application/json'

echo -e '\n-- show db1'
curl --request GET \
     --url http://localhost:23820/databases/db1 \
     --header 'accept: application/json'

echo -e '\n-- show db2'
curl --request GET \
     --url http://localhost:23820/databases/db2 \
     --header 'accept: application/json'

# drop db1 and db2
echo -e '\n-- drop db1'
curl --request DELETE \
     --url http://localhost:23820/databases/db1 \
     --header 'accept: application/json' \
     --header 'content-type: application/json' \
     --data ' {"drop_option": "error"} '

echo -e '\n-- drop db2'
curl --request DELETE \
     --url http://localhost:23820/databases/db2 \
     --header 'accept: application/json' \
     --header 'content-type: application/json' \
     --data ' {"drop_option": "error"} '

# list databases, 'db1' and 'db2' are dropped.
echo -e '\n-- list databases, db1 and db2 are dropped'
curl --request GET \
     --url http://localhost:23820/databases \
     --header 'accept: application/json'
echo -e '\n'