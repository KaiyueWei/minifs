
#!/usr/bin/env python3
"""fs_trace_vis.py – render TinyFS JSON trace to SVG via Graphviz"""
import json, sys
from graphviz import Digraph

def build(trace):
    g=Digraph("fs"); g.attr(rankdir="LR")
    for e in trace:
        nid=f"ev{e['seq']}"
        g.node(nid,f"{e['seq']}: {e['op']}\n{e.get('path','')}",shape="box",style="filled",fillcolor="#eef")
        for blk in e.get("blocks_read",[]):
            bid=f"blk{blk}"
            g.node(bid,f"blk {blk}",shape="ellipse",fillcolor="#cfc",style="filled")
            g.edge(nid,bid,color="blue",label="read")
        for blk in e.get("blocks_written",[]):
            bid=f"blk{blk}"
            g.node(bid,f"blk {blk}",shape="ellipse",fillcolor="#fcc",style="filled")
            g.edge(nid,bid,color="red",label="write")
    return g

if __name__=="__main__":
    if len(sys.argv)<3:
        print("Usage: fs_trace_vis.py trace.json out_svg_basename")
        sys.exit(1)
    with open(sys.argv[1]) as f: trace=json.load(f)
    g=build(trace)
    g.render(sys.argv[2],format="svg",cleanup=True)
