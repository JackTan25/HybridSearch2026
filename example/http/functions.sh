

# create table 'tbl1'
echo -e '-- create tbl1'
curl --request POST \
     --url http://localhost:23820/databases/default_db/tables/tbl1 \
     --header 'accept: application/json' \
     --header 'content-type: application/json' \
     --data ' {
         "create_option": "ignore_if_exists",
         "fields": [
             {
                  "name": "num",
                  "type": "integer"
             },
             {
                  "name": "body",
                  "type": "varchar"
             },
             {
                  "name": "vec",
                  "type": "vector, 4, float"
             },
             {
                  "name": "sparse_column",
                  "type": "sparse,100,float,int"
             },
             {
                  "name": "year",
                  "type": "integer"
             },
             {
                  "name": "tensor",
                  "type": "tensor,4,float"
             },
             {
                  "name": "decimal",
                  "type": "double"
             }
        ]
    } '

# insert rows into 'tbl1'
echo -e '\n\n-- insert rows into tbl1'
curl --request POST \
     --url http://localhost:23820/databases/default_db/tables/tbl1/docs \
     --header 'accept: application/json' \
     --header 'content-type: application/json' \
     --data '
     [
         {
             "num": 1,
             "body": "1234",
             "vec": [1.0, 1.2, 0.8, 0.9],
             "sparse_column": {"10":1.1, "20":2.2, "30": 3.3},
             "year": 2024,
             "tensor": [[1.0, 0.0, 0.0, 0.0], [1.1, 0.0, 0.0, 0.0]],
             "decimal": 1.4
         },
         {
             "num": 2,
             "body": "12345",
             "vec": [4.0, 4.2, 4.3, 4.5],
             "sparse_column": {"40":4.4, "50":5.5, "60": 6.6},
             "year": 2023,
             "tensor": [[4.0, 0.0, 4.3, 4.5], [4.0, 4.2, 4.4, 5.0]],
             "decimal": 1.5
         },
         {
             "num": 3,
             "body": "123456",
             "vec": [4.0, 4.2, 4.3, 4.2],
             "sparse_column":  {"70":7.7, "80":8.8, "90": 9.9},
             "year": 2019,
             "tensor": [[0.9, 0.1, 0.0, 0.0], [1.1, 0.0, 0.0, 0.0]],
             "decimal": -1.4
         },
         {
             "num": 4,
             "body": "123456789",
             "vec": [4.0, 4.2, 4.3, 4.5],
             "sparse_column":  {"20":7.7, "80":7.8, "90": 97.9},
             "year": 2018,
             "tensor": [[5.0, 4.2, 4.3, 4.5], [4.0, 4.2, 4.3, 4.4]],
             "decimal": -1.5
         },
         {
             "num": 5,
             "body": "",
             "vec": [4.0, 4.2, 4.3, 4.5],
             "sparse_column":  {"20":7.7, "80":7.8, "90": 97.9},
             "year": 2018,
             "tensor": [[5.0, 4.2, 4.3, 4.5], [4.0, 4.2, 4.3, 4.4]],
             "decimal": 1
         },
         {
             "num": 6,
             "body": "test@hotmailcom",
             "vec": [4.0, 4.2, 4.3, 4.5],
             "sparse_column":  {"20":7.7, "80":7.8, "90": 97.9},
             "year": 2018,
             "tensor": [[5.0, 4.2, 4.3, 4.5], [4.0, 4.2, 4.3, 4.4]],
             "decimal": -1
         },
         {
             "num": 7,
             "body": "this is a sentence including a mail address, test@hotmail.com",
             "vec": [4.0, 4.2, 4.3, 4.5],
             "sparse_column":  {"20":7.7, "80":7.8, "90": 97.9},
             "year": 2018,
             "tensor": [[5.0, 4.2, 4.3, 4.5], [4.0, 4.2, 4.3, 4.4]],
             "decimal": 0.4
         },
         {
             "num": 8,
             "body": " this is an example for trim ",
             "vec": [4.0, 4.2, 4.3, 4.5],
             "sparse_column":  {"20":7.7, "80":7.8, "90": 97.9},
             "year": 2018,
             "tensor": [[5.0, 4.2, 4.3, 4.5], [4.0, 4.2, 4.3, 4.4]],
             "decimal": 0.5
         }
     ] '

# show all rows of 'tbl1'
echo -e '\n\n-- select all rows of tbl1'
curl --request GET \
     --url http://localhost:23820/databases/default_db/tables/tbl1/docs \
     --header 'accept: application/json' \
     --header 'content-type: application/json' \
     --data '
     {
         "output":
         [
             "*"
         ]
     } '

# show rows of 'tbl1' where char_length(body) = 4
echo -e '\n\n-- select rows of tbl1 where char_length(body) = 4'
curl --request GET \
     --url http://localhost:23820/databases/default_db/tables/tbl1/docs \
     --header 'accept: application/json' \
     --header 'content-type: application/json' \
     --data '
     {
         "output":
         [
             "*"
         ],
         "filter": "char_length(body) = 4"
     } '

# show rows of 'tbl1' where char_length(body) > 4
echo -e '\n\n-- select rows of tbl1 where char_length(body) > 4'
curl --request GET \
     --url http://localhost:23820/databases/default_db/tables/tbl1/docs \
     --header 'accept: application/json' \
     --header 'content-type: application/json' \
     --data '
     {
         "output":
         [
             "*"
         ],
        "filter": "char_length(body) > 4"
     } '

# show rows of 'tbl1' where body inluding 'com'
echo -e '\n\n-- show rows of 'tbl1' where body inluding '\'com\'''
curl --request GET \
     --url http://localhost:23820/databases/default_db/tables/tbl1/docs \
     --header 'accept: application/json' \
     --header 'content-type: application/json' \
     --data '
     {
         "output":
         [
             "body"
         ],
        "filter": "regex(body, '\'com\'')"
     } '

# show rows of 'tbl1' where body including a mail address (using regex)
echo -e '\n\n-- show rows of 'tbl1' where body including a mail address (using regex)'
curl --request GET \
     --url http://localhost:23820/databases/default_db/tables/tbl1/docs \
     --header 'accept: application/json' \
     --header 'content-type: application/json' \
     --data '
     {
         "output":
         [
             "body"
         ],
        "filter": "regex(body, '\''('.'*'')'@'('.'*'')''\\''.'com\'')"
     } '

# show rows of 'tbl1' where body including a mail address (using regex)
echo -e '\n\n-- show rows of 'tbl1' where body including a mail address (using regex)'
curl --request GET \
     --url http://localhost:23820/databases/default_db/tables/tbl1/docs \
     --header 'accept: application/json' \
     --header 'content-type: application/json' \
     --data '
     {
         "output":
         [
             "body"
         ],
        "filter": "regex(body, '\''('[0-9A-Za-z_]+'('[-+.][0-9A-Za-z_]+')''*'')'@'('[0-9A-Za-z_]+'('[-.][0-9A-Za-z_]+')''*'')''\\'.'('[0-9A-Za-z_]+'('[-.][0-9A-Za-z_]+')''*'')'\'')"
     } '

# show rows of 'tbl1' where first 4 chars of body is 'test'
echo -e '\n\n-- show rows of 'tbl1' where first 4 chars of body is 'test''
curl --request GET \
     --url http://localhost:23820/databases/default_db/tables/tbl1/docs \
     --header 'accept: application/json' \
     --header 'content-type: application/json' \
     --data '
     {
         "output":
         [
             "body"
         ],
        "filter": "substring(body, 0, 4) = '\'test\''"
     } '

# show rows of 'tbl1' where upper chars of body is ''
echo -e '\n\n-- show rows of 'tbl1' where upper chars of body is '''
curl --request GET \
     --url http://localhost:23820/databases/default_db/tables/tbl1/docs \
     --header 'accept: application/json' \
     --header 'content-type: application/json' \
     --data '
     {
         "output":
         [
             "body"
         ],
        "filter": "upper(body) = '\'\''"
     } '

# show rows of 'tbl1' where chars of body is lower of ''
echo -e '\n\n-- show rows of 'tbl1' where chars of body is lower of '''
curl --request GET \
     --url http://localhost:23820/databases/default_db/tables/tbl1/docs \
     --header 'accept: application/json' \
     --header 'content-type: application/json' \
     --data '
     {
         "output":
         [
             "body"
         ],
        "filter": "body = lower('\'\'')"
     } '

# show rows of 'tbl1' where ltrim(body) is "this is an example for trim "
echo -e '\n\n-- show rows of 'tbl1' where ltrim(body) is "this is an example for trim "'
curl --request GET \
     --url http://localhost:23820/databases/default_db/tables/tbl1/docs \
     --header 'accept: application/json' \
     --header 'content-type: application/json' \
     --data '
     {
         "output":
         [
             "body"
         ],
        "filter": "ltrim(body) = '\''this is an example for trim '\''"
     } '

# show rows of 'tbl1' where rtrim(body) is " this is an example for trim"
echo -e '\n\n-- show rows of 'tbl1' where rtrim(body) is " this is an example for trim"'
curl --request GET \
     --url http://localhost:23820/databases/default_db/tables/tbl1/docs \
     --header 'accept: application/json' \
     --header 'content-type: application/json' \
     --data '
     {
         "output":
         [
             "body"
         ],
        "filter": "rtrim(body) = '\'' this is an example for trim'\''"
     } '

# show rows of 'tbl1' where trim(body) is "this is an example for trim "
echo -e '\n\n-- show rows of 'tbl1' where trim(body) is "this is an example for trim"'
curl --request GET \
     --url http://localhost:23820/databases/default_db/tables/tbl1/docs \
     --header 'accept: application/json' \
     --header 'content-type: application/json' \
     --data '
     {
         "output":
         [
             "body"
         ],
        "filter": "trim(body) = '\''this is an example for trim'\''"
     } '

# show rows of 'tbl1' where char_position(body, '123') = 1
echo -e '\n\n-- show rows of 'tbl1' where char_position(body, '123') = 1'
curl --request GET \
     --url http://localhost:23820/databases/default_db/tables/tbl1/docs \
     --header 'accept: application/json' \
     --header 'content-type: application/json' \
     --data '
     {
         "output":
         [
             "body"
         ],
        "filter": "char_position(body, '123') = 1"
     } '

# show rows of 'tbl1' with sqrt(num)
echo -e '\n\n-- show rows of 'tbl1' with sqrt(num)'
curl --request GET \
     --url http://localhost:23820/databases/default_db/tables/tbl1/docs \
     --header 'accept: application/json' \
     --header 'content-type: application/json' \
     --data '
     {
         "output":
         [
             "num", "sqrt(num)"
         ]
     } '

# show rows of 'tbl1' with decimal, round(decimal), ceil(decimal), floor(decimal)
echo -e '\n\n-- show rows of 'tbl1' with decimal, round(decimal), ceil(decimal), floor(decimal)'
curl --request GET \
     --url http://localhost:23820/databases/default_db/tables/tbl1/docs \
     --header 'accept: application/json' \
     --header 'content-type: application/json' \
     --data '
     {
         "output":
         [
             "decimal", "round(decimal)", "ceil(decimal)", "floor(decimal)"
         ]
     } '

# show rows of 'tbl1' with num, ln(num)
echo -e '\n\n-- show rows of 'tbl1' with decimal, round(decimal), ceil(decimal), floor(decimal)'
curl --request GET \
     --url http://localhost:23820/databases/default_db/tables/tbl1/docs \
     --header 'accept: application/json' \
     --header 'content-type: application/json' \
     --data '
     {
         "output":
         [
             "num", "ln(num)"
         ]
     } '


# drop tbl1
echo -e '\n\n-- drop tbl1'
curl --request DELETE \
     --url http://localhost:23820/databases/default_db/tables/tbl1 \
     --header 'accept: application/json' \
     --header 'content-type: application/json' \
     --data ' {"drop_option": "error"} '
echo -e '\n'