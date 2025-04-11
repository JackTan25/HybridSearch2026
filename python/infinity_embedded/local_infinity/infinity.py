

import os
from hybridsearch_embedded import hybridsearchConnection
from abc import ABC
from hybridsearch_embedded.common import ConflictType, LOCAL_hybridsearch_PATH, hybridsearchException, LOCAL_hybridsearch_CONFIG_PATH
from hybridsearch_embedded.local_hybridsearch.client import LocalhybridsearchClient
from hybridsearch_embedded.embedded_hybridsearch_ext import ConflictType as LocalConflictType
from hybridsearch_embedded.errors import ErrorCode
from hybridsearch_embedded.local_hybridsearch.db import LocalDatabase
from hybridsearch_embedded.local_hybridsearch.utils import name_validity_check
import logging


class LocalhybridsearchConnection(hybridsearchConnection, ABC):
    def __init__(self, uri=LOCAL_hybridsearch_PATH, config_path=LOCAL_hybridsearch_CONFIG_PATH):
        if not os.path.exists(uri):
            try:
                logging.warning(f"Directory {uri} not found, try to create it")
                os.makedirs(uri)
            except OSError as e:
                raise hybridsearchException(ErrorCode.DIR_NOT_FOUND, f"Directory {uri} not found and create failed: {e}")
        if os.path.isdir(uri):
            if os.access(uri, os.R_OK | os.W_OK):
                self._client = LocalhybridsearchClient(uri, config_path)
                self._is_connected = True
            else:
                raise hybridsearchException(ErrorCode.UNEXPECTED_ERROR,
                                        f"Directory {uri} does not have read or write permissions")
        else:
            raise hybridsearchException(ErrorCode.DIR_NOT_FOUND, f"Directory {uri} not found")

        self.db_name = "default_db"

    def __del__(self):
        if self._is_connected is True:
            self.disconnect()

    def check_connect(self):
        if (self._is_connected is False):
            raise Exception("Local hybridsearch is not connected")

    @name_validity_check("db_name", "DB")
    def create_database(self, db_name: str, conflict_type: ConflictType = ConflictType.Error, comment: str = None):
        self.check_connect()
        create_database_conflict: LocalConflictType
        if conflict_type == ConflictType.Error:
            create_database_conflict = LocalConflictType.kError
        elif conflict_type == ConflictType.Ignore:
            create_database_conflict = LocalConflictType.kIgnore
        elif conflict_type == ConflictType.Replace:
            create_database_conflict = LocalConflictType.kReplace
        else:
            raise hybridsearchException(ErrorCode.INVALID_CONFLICT_TYPE, "Invalid conflict type")
        res = self._client.create_database(db_name, create_database_conflict, comment)

        if res.error_code == ErrorCode.OK:
            return LocalDatabase(self._client, db_name)
        else:
            raise hybridsearchException(res.error_code, res.error_msg)

    def list_databases(self):
        self.check_connect()
        res = self._client.list_databases()
        if res.error_code == ErrorCode.OK:
            return res
        else:
            raise hybridsearchException(res.error_code, res.error_msg)

    @name_validity_check("db_name", "DB")
    def show_database(self, db_name):
        self.check_connect()
        res = self._client.show_database(db_name)
        if res.error_code == ErrorCode.OK:
            return res
        else:
            raise hybridsearchException(res.error_code, res.error_msg)

    def show_info(self, info_name):
        self.check_connect()
        res = self._client.show_info(info_name)
        if res.error_code == ErrorCode.OK:
            return res
        else:
            raise hybridsearchException(res.error_code, res.error_msg)

    def show_current_node(self):
        self.check_connect()
        res = self._client.show_current_node()
        if res.error_code == ErrorCode.OK:
            return res
        else:
            raise hybridsearchException(res.error_code, res.error_msg)

    def search(self, db_name, table_name):
        self.check_connect()
        res = self._client.search(db_name, table_name, [])
        if res.error_code == ErrorCode.OK:
            return res
        else:
            raise hybridsearchException(res.error_code, res.error_msg)

    @name_validity_check("db_name", "DB")
    def drop_database(self, db_name, conflict_type: ConflictType = ConflictType.Error):
        self.check_connect()
        drop_database_conflict: LocalConflictType
        if conflict_type == ConflictType.Error:
            drop_database_conflict = LocalConflictType.kError
        elif conflict_type == ConflictType.Ignore:
            drop_database_conflict = LocalConflictType.kIgnore
        else:
            raise hybridsearchException(ErrorCode.INVALID_CONFLICT_TYPE, "Invalid conflict type")

        res = self._client.drop_database(db_name, drop_database_conflict)
        if res.error_code == ErrorCode.OK:
            return res
        else:
            raise hybridsearchException(res.error_code, res.error_msg)

    @name_validity_check("db_name", "DB")
    def get_database(self, db_name):
        self.check_connect()
        res = self._client.get_database(db_name)
        if res.error_code == ErrorCode.OK:
            return LocalDatabase(self._client, db_name)
        else:
            raise hybridsearchException(res.error_code, res.error_msg)

    def disconnect(self):
        res = self._client.disconnect()
        if res.error_code == ErrorCode.OK:
            self._is_connected = False
            return res
        else:
            raise hybridsearchException(res.error_code, res.error_msg)

    def client(self):
        return self._client
