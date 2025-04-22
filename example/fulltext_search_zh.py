

"""
This example is to connect local hybridsearch instance, create table, insert data, search the data which Chinese words
"""

"""
Checkout https://github.com/hybridsearchflow/resource.git under /var/hybridsearch (defined by 'resource_dir' of config file). The jieba dict is
/var/hybridsearch/resource/jieba/dict/jieba.dict.utf8
"""
# import hybridsearch_embedded as hybridsearch
import hybridsearch
import sys

try:
    # Use hybridsearch_embedded module to open a local directory
    # hybridsearch_instance = hybridsearch.connect("/var/hybridsearch")

    #  Use hybridsearch module to connect a remote server
    hybridsearch_instance = hybridsearch.connect(hybridsearch.common.NetworkAddress("127.0.0.1", 23817))

    # 'default_db' is the default database
    db_instance = hybridsearch_instance.get_database("default_db")

    # Drop my_table if it already exists
    db_instance.drop_table("my_table", hybridsearch.common.ConflictType.Ignore)

    # Create a table named "my_table"
    table_instance = db_instance.create_table("my_table", {
        "num": {"type": "integer"},
        "body": {"type": "varchar"},
        "vec": {"type": "vector, 4, float"},
    })

    # Insert 3 rows of data into the 'my_table'
    table_instance.insert(
        [
            {
                "num": 1,
                "body": r"WccftechAITSMC3nm2025N3E",
                "vec": [1.0, 1.2, 0.8, 0.9],
            },
            {
                "num": 2,
                "body": r"“Project Titan”2000AI",
                "vec": [1.2, 0.8, 0.9, 1.0],
            },
            {
                "num": 3,
                "body": r"",
                "vec": [0.8, 0.9, 1.0, 1.2],
            },
            {
                "num": 4,
                "body": r"“”“2018”“NJ”[3]20182018“”20187307340“”[6]20187172018",
                "vec": [0.9, 1.0, 1.2, 0.8],
            },
            {
                "num": 5,
                "body": r"“”“”“hardware”“motherboard”“graphics card”“awesome”“peripheral devices”“keyboard”“mouse”“power supply”“high-tech”“storage devices”“happily”",
                "vec": [1.0, 0.9, 0.8, 0.9],
            },
            {
                "num": 6,
                "body": r"  Model NameThinkBook 16 G5+ ARP  /20V=5A  CMIT ID2023AP123456  MO: DS-K3AJ303/Dm140",
                "vec": [0.9, 0.8, 0.9, 1.0],
            },
            {
                "num": 7,
                "body": r"Office for Harmful Blooms",
                "vec": [4.0, 4.2, 4.3, 4.5],
            },
            {
                "num": 8,
                "body": r"A Bloom filter is a space-efficient probabilistic data structure, conceived by Burton Howard Bloom in 1970, that is used to test whether an element is a member of a set.",
                "vec": [4.0, 4.2, 4.3, 4.5],
            },
        ]
    )

    # Create index on varchar column for full-text search and multiple way fusion.
    res = table_instance.create_index(
        "my_index",
        hybridsearch.index.IndexInfo("body", hybridsearch.index.IndexType.FullText, {"ANALYZER": "chinese"}), hybridsearch.common.ConflictType.Error)

    questions = [
        r"",  # single term
        r"",  # single term
        r"",  # single term
        r"",  # single term
        r'""',  # phrase: adjacent multiple terms
        r"2018",  # OR multiple terms
        r"high\:tech",  # Escape reserved character ':'
        r'"high tech"',  # phrase: adjacent multiple terms
        r'"high-tech"',  # phrase: adjacent multiple terms
        r"graphics card",  # OR multiple terms
        r'"graphics card"',  # phrase: adjacent multiple terms
        r'"DS-K3AJ303/Dm140"',  # phrase: adjacent multiple terms
        r"Bloom filter",  # OR multiple terms
        r'"Bloom filter"',  # phrase: adjacent multiple terms
    ]
    for question in questions:
        qb_result, extra_result = table_instance.output(["num", "body", "_score"]).highlight(["body"]).match_text("body", question, 10).to_pl()
        print(f"question: {question}")
        print(qb_result)
        if extra_result is not None:
            print(extra_result)

    hybridsearch_instance.disconnect()

    print('test done')
    sys.exit(0)
except Exception as e:
    print(str(e))
    sys.exit(-1)