import pandas as pd
import os

import time

def top_10_costliest_tips():
    # artificail delay
    time.sleep(0.5)
    url = "https://raw.githubusercontent.com/mwaskom/seaborn-data/master/tips.csv"
    file_path = "tmp_DMPV_output.raw"
    #Sort Data
    data = pd.read_csv(url)
    print("data: ok")
    sorted_data = data[['day' , 'tip']].sort_values(by='tip', ascending=False).head(100)

    sorted_data2 = sorted_data.groupby('day', as_index=False)['tip'].sum()

    with open("tmp_DMPV_output.raw", "w") as k:
        for i, row in enumerate(sorted_data2.itertuples(index=False) , start=1):
            k.write(str(f"{row.day} {row.tip}\n"))
    

top_10_costliest_tips()
