#!/bin/python3

import numpy as np
import pandas as pd
import seaborn as sns
import matplotlib as mpl
import matplotlib.pyplot as plt

if __name__=='__main__':
    pd.options.display.float_format='{:.0f}'.format

    f=open('cost.32.MT.log','r')
    MT32=[round(float(x)/24) for x in f.read().split('\n')[:-1]]

    f=open('cost.32.rand.log','r')
    rand32=[round(float(x)/24) for x in f.read().split('\n')[:-1]]

    f=open('cost.64.MT.log','r')
    MT64=[round(float(x)/24) for x in f.read().split('\n')[:-1]]

    f=open('cost.64.rand.log','r')
    rand64=[round(float(x)/24) for x in f.read().split('\n')[:-1]]

    times32=MT32+rand32
    gen32=["Mersenne Twister"]*1000+["C rand"]*1000

    df32={
            "CPU Time (days)":times32,
            "Generator":gen32
            }

    times64=MT64+rand64
    gen64=["Mersenne Twister"]*1000+["C rand"]*1000

    df64={
            "CPU Time (days)":times64,
            "Generator":gen64
            }
    
    fig, axes=plt.subplots(2, 1)
    fig.tight_layout(pad=5.0)
    fig.suptitle("Estimated Sizes of Exact Cover Searches")
    axes[0].set_title("Order 32 Affine Planes")
    axes[1].set_title("Order 64 Affine Planes")

    #hatch=['/', '|']

    p1=sns.violinplot(ax=axes[0], data=df32, x="CPU Time (days)", y="Generator", hue="Generator")
    #ihatch=iter(hatch)
    #_=[i.set_hatch(next(ihatch)) for i in p1.get_children() if isinstance(i, mpl.collections.PolyCollection)]

    p2=sns.violinplot(ax=axes[1], data=df64, x="CPU Time (days)", y="Generator", hue="Generator")

    plt.show()
