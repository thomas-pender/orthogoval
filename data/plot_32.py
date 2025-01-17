#!/bin/python3

import numpy as np
import pandas as pd
import seaborn as sns
import matplotlib as mpl
import matplotlib.pyplot as plt

if __name__=='__main__':
    pd.options.display.float_format='{:.0f}'.format

    f=open('cost.32.MT.log','r')
    AMT=[z.split() for z in f.read().split('\n')[:-1]]
    MT32_time=[round(float(a[0])/24) for a in AMT]
    MT32_size=[int(a[1])/1000000000 for a in AMT]

    f=open('cost.32.rand.log','r')
    Arand=[z.split() for z in f.read().split('\n')[:-1]]
    rand32_time=[round(float(a[0])/24) for a in Arand]
    rand32_size=[int(a[1])/1000000000 for a in Arand]

    sizes32=MT32_size+rand32_size
    times32=MT32_time+rand32_time
    gen32=["Mersenne Twister"]*1000+["C rand"]*1000

    data32={
        "Tree Size (billions of nodes)":sizes32,
        "CPU Time (days)":times32,
        "Generator":gen32
    }
    df32=pd.DataFrame(data32)

    fig, axes=plt.subplots(2, 1)
    fig.tight_layout(pad=5.0)
    #fig.suptitle("Estimated Sizes of Exact Cover Searches")
    axes[0].set_title("Order 32 Search: CPU Time")
    axes[1].set_title("Order 32 Search: Search Tree Size")

    sns.set_palette("Set2")

    sns.violinplot(ax=axes[0], data=df32, x="CPU Time (days)", y="Generator", hue="Generator")
    sns.violinplot(ax=axes[1], data=df32, x="Tree Size (billions of nodes)", y="Generator", hue="Generator")

    plt.show()
