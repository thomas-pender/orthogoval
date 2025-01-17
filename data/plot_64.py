#!/bin/python3

import numpy as np
import pandas as pd
import seaborn as sns
import matplotlib as mpl
import matplotlib.pyplot as plt

if __name__=='__main__':
    pd.options.display.float_format='{:.0f}'.format

    f=open('cost.64.MT.log','r')
    AMT=[z.split() for z in f.read().split('\n')[:-1]]
    MT64_time=[round(float(a[0])/24) for a in AMT]
    MT64_size=[int(a[1])/1000000000 for a in AMT]

    f=open('cost.64.rand.log','r')
    Arand=[z.split() for z in f.read().split('\n')[:-1]]
    rand64_time=[round(float(a[0])/24) for a in Arand]
    rand64_size=[int(a[1])/1000000000 for a in Arand]

    sizes64=MT64_size+rand64_size
    times64=MT64_time+rand64_time
    gen64=["Mersenne Twister"]*1000+["C rand"]*1000

    data64={
        "Tree Size (billions of nodes)":sizes64,
        "CPU Time (days)":times64,
        "Generator":gen64
    }
    df64=pd.DataFrame(data64)

    fig, axes=plt.subplots(2, 1)
    fig.tight_layout(pad=5.0)
    #fig.suptitle("Estimated Sizes of Exact Cover Searches")
    axes[0].set_title("Order 64 Search: CPU Time")
    axes[1].set_title("Order 64 Search: Search Tree Size")

    sns.set_palette("Set2")

    sns.violinplot(ax=axes[0], data=df64, x="CPU Time (days)", y="Generator", hue="Generator")
    sns.violinplot(ax=axes[1], data=df64, x="Tree Size (billions of nodes)", y="Generator", hue="Generator")

    plt.show()
