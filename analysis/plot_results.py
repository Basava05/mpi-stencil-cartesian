#!/usr/bin/env python3
from pathlib import Path
import csv
import matplotlib.pyplot as plt
RAW,OUT=Path('results/raw'),Path('results/figures'); OUT.mkdir(parents=True,exist_ok=True)
def load(n):
 p=RAW/n
 return list(csv.DictReader(p.open())) if p.exists() else []
def draw(name,title,xlabel,ylabel,groups):
 if not groups:return
 plt.figure()
 for label,vals in groups.items():
  vals=sorted(vals)
  if vals:plt.plot(*zip(*vals),marker='o',label=label)
 plt.title(title);plt.xlabel(xlabel);plt.ylabel(ylabel);plt.grid(alpha=.3);plt.legend();plt.tight_layout();plt.savefig(OUT/name,dpi=160);plt.close()
strong=load('strong_scaling_1d.csv')+load('strong_scaling_2d.csv')
for field,name,title,y in [('surface_to_volume','surface_to_volume_vs_processes.png','Surface-to-volume ratio','Ratio'),('seconds','strong_scaling.png','Strong scaling','Execution time (s)')]:
 draw(name,title,'Processes',y,{d:[(int(r['processes']),float(r[field])) for r in strong if r['decomp']==d] for d in ('1d','2d')})
ov=load('overlap.csv'); pairs={}
for r in ov:pairs.setdefault((r['decomp'],r['processes']),{})[r['mode']]=float(r['seconds'])
sp={'1d':[],'2d':[]}
for (d,p),v in pairs.items():
 if 'blocking'in v and 'overlap'in v:sp[d].append((int(p),v['blocking']/v['overlap']))
draw('overlap_speedup.png','Overlap speedup','Processes','Blocking / overlap time',sp)
draw('decomposition_comparison.png','1-D vs 2-D decomposition','Processes','Execution time (s)',{d:[(int(r['processes']),float(r['seconds'])) for r in strong if r['decomp']==d and r['processes']=='4'] for d in ('1d','2d')})
weak=load('weak_scaling.csv')
draw('weak_scaling.png','Weak scaling','Processes','Execution time (s)',{d:[(int(r['processes']),float(r['seconds'])) for r in weak if r['decomp']==d] for d in ('1d','2d')})
red=load('reduction_cost.csv')
draw('reduction_interval_cost.png','Residual-checking overhead','Check interval K','Time (s)',{'execution':[(int(r['check_every']) if 'check_every'in r else 0,float(r['seconds'])) for r in red],'MPI_Allreduce':[(int(r['check_every']) if 'check_every'in r else 0,float(r['reduction_seconds'])) for r in red]})
