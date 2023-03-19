import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import akshare as ak
import os

# A股历史行情数据，period=daily,weekly,monthly;adjust=qfq前复权之后的数据，hfq复权之后的数据
if os.path.exists('class1_1.csv') == False:
    df = ak.stock_zh_a_hist(symbol='600519', period='daily', start_date='20060101', end_date='20220916', adjust='hfq')
    df.to_csv('class1_1.csv')

df_data = pd.read_csv('class1_1.csv', index_col='日期', parse_dates=['日期'])[['开盘','收盘','最高','最低']]
print(df_data)

# 收盘比开盘高3%
print(df_data[(df_data['收盘'] - df_data['开盘']) / df_data['开盘'] >= 0.03])

# 当日开盘比前日收盘跌2%
print(df_data[(df_data['开盘'] - df_data['收盘'].shift(1)) / df_data['收盘'].shift(1) <= -0.02])

# 每个月第一天买入，每年最后一个交易日卖出，收益如何 2006-2021
df_filter = df_data['2006-01-01':'2021-12-31']
df_monthly = df_filter.resample('M').first()
df_monthly = df_monthly[df_monthly.isnull().any(axis=1) == False]
df_yearly = df_filter.resample('A').last()

cost_money = 0
for year in range(2006, 2022):
    cost_money += df_monthly.loc[str(year)]['开盘'].sum() * 100
    hold =  len(df_monthly.loc[str(year)]) * 100
    print("cost:", cost_money, hold)
    
    cost_money -= df_yearly.loc[str(year)]['开盘'][0] * hold
    print("sell:",  cost_money)
