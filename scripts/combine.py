#!/usr/bin/env python

import argparse
import collections
import os
import pandas
import six

OUTFILE="out.csv"

def parse(root):
    dfs = collections.defaultdict(list)
    for subdirs, dirs, files in os.walk(root):
        for f in files:
            if f.startswith("abort") and f.endswith(".csv"):
                fullpath = os.path.join(subdirs, f)
                print(fullpath)

                assert subdirs.startswith(root)
                key = int(subdirs[len(root) + (0 if root[-1] == '/' else 1) : ])
                dfs[key].append(pandas.read_csv(filepath_or_buffer=fullpath,header=0,index_col=0).apply(pandas.to_numeric,errors="coerce"))

    output = collections.OrderedDict()
    for k, v in sorted(six.iteritems(dfs)):
        concat = pandas.concat(v,copy=False)
        for c in ["Time", "TAborts", "TThroughput"]:
            if (c in concat.index):
                if (type(concat.loc[c]) == pandas.core.series.Series):
                    row = concat.loc[c].to_frame().transpose()
                    row.index.name = "Benchmark"
                else:
                    row = concat.loc[c]

                for o in ["count", "mean", "std", "min", "max"]:
                    output["-".join([str(k), c, o])] = getattr(row, o)()

    outdata = pandas.DataFrame(output,columns=output.keys(),copy=False).transpose()
    outdata.to_csv(path_or_buf=os.path.join(root, OUTFILE))

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("directory")
    args = parser.parse_args()

    parse(args.directory)
