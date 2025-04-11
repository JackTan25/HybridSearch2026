

from abc import ABC, abstractmethod


# abstract class
class hybridsearchConnection(ABC):
    def __init__(self, uri):
        self.uri = uri

    @abstractmethod
    def create_database(self, db_name, options=None, comment: str = None):
        pass

    @abstractmethod
    def list_databases(self):
        pass

    @abstractmethod
    def show_database(self, db_name):
        pass

    @abstractmethod
    def drop_database(self, db_name, options=None):
        pass

    @abstractmethod
    def get_database(self, db_name):
        pass

    @abstractmethod
    def disconnect(self):
        pass
