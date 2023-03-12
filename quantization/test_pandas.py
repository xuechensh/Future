import pandas as pd

s = pd.Series([1,2,3,4,5])
print(s)

print(s.iloc[3]) # 中括号内的整数被解释为下标

sr1 = pd.Series([1,2,3], index = ['a', 'b', 'c'])
sr2 = pd.Series([4,5,6], index = ['c', 'a', 'b'])
print(sr1 + sr2)
# a    6
# b    8
# c    7
# dtype: int64

sr3 = pd.Series([7,8,9], index = ['b','c','d'])
sr4 = sr1 + sr3
print(sr4)
# a     NaN
# b     9.0
# c    11.0
# d     NaN
# dtype: float64

sr5 = sr1.add(sr3, fill_value = 0)
print(sr5)
# a     1.0
# b     9.0
# c    11.0
# d     9.0
# dtype: float64

print(sr4.fillna(0))
print(sr4.dropna())

# DataFrame
df1 = pd.DataFrame({'one':[1,2,3], 'two':[4,5,6]}, index = ['a', 'b', 'c'])
print(df1)
print(df1.loc['a', 'one'])
