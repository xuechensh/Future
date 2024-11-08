import backtrader as bt
import yfinance as yf
import datetime

# 创建一个交易策略
class MovingAverageCrossStrategy(bt.Strategy):
    # 设置均线的参数
    short_period = 50
    long_period = 200
    
    # 初始化方法，定义移动均线指标
    def __init__(self):
        self.short_ma = bt.indicators.SimpleMovingAverage(self.data.close, period=self.short_period)
        self.long_ma = bt.indicators.SimpleMovingAverage(self.data.close, period=self.long_period)
    
    # 定义买卖信号
    def next(self):
        if self.short_ma > self.long_ma:
            if not self.position:
                self.buy()
        elif self.short_ma < self.long_ma:
            if self.position:
                self.sell()

# 下载数据
data = yf.download('AAPL', start='2010-01-01', end='2020-01-01')

# 检查数据类型，确保返回的是 DataFrame
if isinstance(data, tuple):
    print("Error: yfinance returned a tuple, check the data source.")
else:
    print("Data successfully loaded as DataFrame.")

# 打印数据，确保它是 pandas DataFrame
print(data.head())

# 确保数据是 DataFrame 格式，并传递给 backtrader 的 PandasData
data_feed = bt.feeds.PandasData(dataname=data)

# 创建回测引擎
cerebro = bt.Cerebro()

# 加载数据
cerebro.adddata(data_feed)

# 添加策略
cerebro.addstrategy(MovingAverageCrossStrategy)

# 设置初始资金
cerebro.broker.set_cash(100000)

# 设置佣金，使用 CommissionInfo
commission_info = bt.broker.CommInfoBase(commission=0.001)  # 设置佣金比例为每笔交易金额的千分之一
cerebro.broker.addcommissioninfo(commission_info)

# 设置滑点
cerebro.broker.set_slippage_perc(0.001)  # 设置滑点为千分之一

# 输出初始资金
print(f"Initial Portfolio Value: {cerebro.broker.getvalue()}")

# 运行回测
try:
    cerebro.run()
except Exception as e:
    print(f"Error during cerebro.run(): {e}")

# 输出回测后的资金
print(f"Final Portfolio Value: {cerebro.broker.getvalue()}")

# 绘制回测结果
cerebro.plot()
