import akshare as ak
ak.__version__

stock_zh_a_hist_df = ak.stock_zh_a_hist(symbol="000001", period="daily", start_date="20220124", end_date='20220125', adjust="")
print(stock_zh_a_hist_df)
