from dbutils.pooled_db import PooledDB
import pymysql

class DBUtilsDemo:
    def __init__(self, url, port, user, password, database):
        db_config = {
            "host": url,
            "port": port,
            "user": user,
            "db": database,
            "password": password,
            "charset": "utf8",
            "connect_timeout": 3,
            "read_timeout": 10,
            "write_timeout": 10
        }
        '''
          DBUtils连接池常见参数的详细说明请参见后文，以下为参数简要说明：
            mincached: 池中闲置连接的初始数量（0表示启动时不会创建连接）。
            maxcached: 池中闲置连接的最大数量（0或None表示连接池大小没有限制）。
            maxshared: 共享连接的最大数量（0或None表示所有连接都是专用的）。
            maxconnections: 一般允许的最大连接数量（0或None表示连接不受限制）。
            blocking: 在超过最大连接数量时的行为。
            maxusage: 单个连接的最大重用次数（0或None表示无限重用）。
        '''
        self.pooled = PooledDB(pymysql, maxcached=20, maxshared=0, maxconnections=100, maxusage=20, **db_config)

    # 从连接池获取连接
    def _connect(self):
        try:
            r = self.pooled.connection()
            return r
        except Exception as e:
            print("Failed to connect:" + str(e))

    # 归还连接到连接池
    def _close(self, conn, stmt):
        if stmt:
            stmt.close()
        if conn:
            conn.close()

    # 查询单条记录
    def select_row(self, sql):
        connection = self._connect()
        statement = None
        try:
            statement = connection.cursor()
            statement.execute(sql)
            row = statement.fetchone()
            return row
        except Exception as e:
            print(e)
        finally:
            self._close(connection, statement)

if __name__ == '__main__':
    # MySQL实例连接地址。
    url = 'localhost'
    # 端口。
    port = 3307
    # 用户名。
    user = 'root'
    # 密码。
    password = 'root'
    # 连接的数据库名称。
    database = 'employees'

    # 获取连接池对象
    poolUtils = DBUtilsDemo(url, port, user, password, database)

    # 查询单条记录
    row = poolUtils.select_row("select * from employees limit 1")
    print(row)
