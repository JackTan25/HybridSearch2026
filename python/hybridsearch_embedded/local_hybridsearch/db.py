

from abc import ABC

from hybridsearch_embedded.db import Database
from hybridsearch_embedded.errors import ErrorCode
from hybridsearch_embedded.local_hybridsearch.table import LocalTable
from hybridsearch_embedded.local_hybridsearch.utils import check_valid_name, name_validity_check
from hybridsearch_embedded.common import ConflictType, hybridsearchException
from hybridsearch_embedded.local_hybridsearch.utils import select_res_to_polars, get_ordinary_info
from hybridsearch_embedded.embedded_hybridsearch_ext import ConflictType as LocalConflictType


class LocalDatabase(Database, ABC):
    def __init__(self, conn, name: str, comment: str = None):
        self._conn = conn
        self._db_name = name
        self._db_comment = comment

    def create_table(self, table_name: str, columns_definition,
                     conflict_type: ConflictType = ConflictType.Error):
        # process column definitions
        """
        db_obj.create_table("my_table",
            {
                "c1": {
                    "type": "int",
                    "constraints":["primary key", ...],
                    "default"(optional): 1/"asdf"/[1,2]/...
                },
                "c2": {
                    "type":"vector,1024,float32",
                }
            }, None)
        """
        # to column_defs
        column_defs = []
        for index, (column_name, column_info) in enumerate(columns_definition.items()):
            check_valid_name(column_name, "Column")
            get_ordinary_info(column_info, column_defs, column_name, index)

        create_table_conflict: LocalConflictType
        if conflict_type == ConflictType.Error:
            create_table_conflict = LocalConflictType.kError
        elif conflict_type == ConflictType.Ignore:
            create_table_conflict = LocalConflictType.kIgnore
        elif conflict_type == ConflictType.Replace:
            create_table_conflict = LocalConflictType.kReplace
        else:
            raise hybridsearchException(ErrorCode.INVALID_CONFLICT_TYPE, f"Invalid conflict type")
        res = self._conn.create_table(db_name=self._db_name, table_name=table_name,
                                      column_defs=column_defs,
                                      conflict_type=create_table_conflict)

        if res.error_code == ErrorCode.OK:
            return LocalTable(self._conn, self._db_name, table_name)
        else:
            raise hybridsearchException(res.error_code, res.error_msg)

    @name_validity_check("table_name", "Table")
    def drop_table(self, table_name, conflict_type: ConflictType = ConflictType.Error):
        drop_table_conflict: LocalConflictType
        if conflict_type == ConflictType.Error:
            drop_table_conflict = LocalConflictType.kError
        elif conflict_type == ConflictType.Ignore:
            drop_table_conflict = LocalConflictType.kIgnore
        else:
            raise hybridsearchException(ErrorCode.INVALID_CONFLICT_TYPE, "nvalid conflict type")
        res = self._conn.drop_table(db_name=self._db_name, table_name=table_name, conflict_type=drop_table_conflict)
        if res.error_code == ErrorCode.OK or res.error_code == ErrorCode.TABLE_NOT_EXIST:
            return res
        else:
            raise hybridsearchException(res.error_code, res.error_msg)

    def list_tables(self):
        res = self._conn.list_tables(self._db_name)
        if res.error_code == ErrorCode.OK:
            return res
        else:
            raise hybridsearchException(res.error_code, res.error_msg)

    @name_validity_check("table_name", "Table")
    def show_table(self, table_name):
        res = self._conn.show_table(db_name=self._db_name, table_name=table_name)
        if res.error_code == ErrorCode.OK:
            return res
        else:
            raise hybridsearchException(res.error_code, res.error_msg)

    @name_validity_check("table_name", "Table")
    def get_table(self, table_name):
        res = self._conn.get_table(db_name=self._db_name, table_name=table_name)
        if res.error_code == ErrorCode.OK:
            return LocalTable(self._conn, self._db_name, table_name)
        else:
            raise hybridsearchException(res.error_code, res.error_msg)

    def show_tables(self):
        res = self._conn.show_tables(self._db_name)
        if res.error_code == ErrorCode.OK:
            return select_res_to_polars(res)
        else:
            raise hybridsearchException(res.error_code, res.error_msg)
