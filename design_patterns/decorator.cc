#include <iostream>
#include <memory>
#include <string>

std::string SERIALIZE_PREFIX("Serialize data: ");
std::string COMPRESS_PREFIX("Compress data: ");

class DataSource {
 public:
  virtual ~DataSource() = default;
  virtual void WriteData(const std::string& data) = 0;
  virtual std::string ReadData() const = 0;
};

class FileDataSource : public DataSource {
 public:
  FileDataSource(const std::string& path) : path_(path), data_(){};

  void WriteData(const std::string& data) override {
    data_ = data;
    std::cout << "Writer to file " << path_ << " data: [" << data_ << "]"
              << std::endl;
  }

  std::string ReadData() const override {
    std::cout << "Read from file " << path_ << " data: [" << data_ << "]"
              << std::endl;
    return data_;
  }

 private:
  std::string path_;
  std::string data_;
};

// 基础套娃类，最底层的套娃类，用于隐藏真实调用对象
class DataSourceDecorator : public DataSource {
 public:
  DataSourceDecorator(const std::shared_ptr<DataSource>& data_source)
      : wrapper_(data_source) {}

  void WriteData(const std::string& data) override {
    wrapper_->WriteData(data);
  }

  std::string ReadData() const override { return wrapper_->ReadData(); }

 private:
  std::shared_ptr<DataSource> wrapper_;
};

class SerializeDecorator : public DataSourceDecorator {
 public:
  SerializeDecorator(const std::shared_ptr<DataSource>& data_source)
      : DataSourceDecorator(data_source) {}

  void WriteData(const std::string& data) override {
    std::string tmp = SERIALIZE_PREFIX + data;
    // 调用父类WriteData函数，父类对象在构造对象的时候在构造函数传递
    DataSourceDecorator::WriteData(tmp);
  }

  std::string ReadData() const override {
    std::string tmp = DataSourceDecorator::ReadData();
    return tmp.substr(SERIALIZE_PREFIX.length());
  }
};

class CompressDecorator : public DataSourceDecorator {
 public:
  CompressDecorator(const std::shared_ptr<DataSource>& data_source)
      : DataSourceDecorator(data_source) {}

  void WriteData(const std::string& data) override {
    std::string tmp = COMPRESS_PREFIX + data;
    // 调用父类WriteData函数，父类对象在构造对象的时候在构造函数传递
    DataSourceDecorator::WriteData(tmp);
  }

  std::string ReadData() const override {
    std::string tmp = DataSourceDecorator::ReadData();
    return tmp.substr(COMPRESS_PREFIX.length());
  }
};

int main() {
  auto file = std::make_shared<FileDataSource>("/tmp/1.txt");
  auto compress = std::make_shared<CompressDecorator>(file);
  compress->WriteData("abcdefg");
  std::cout << compress->ReadData() << std::endl;

  // 最优先调用的逻辑的应该写在最外面封装，就跟套娃一样，从外到内处理
  auto serialize = std::make_shared<SerializeDecorator>(compress);
  serialize->WriteData("abcedfg");
  std::cout << serialize->ReadData() << std::endl;

  return 0;
}
