Add-Type -AssemblyName System.Drawing
$out = Join-Path (Get-Location) 'output'
New-Item -ItemType Directory -Force -Path $out | Out-Null
function Canvas($name, [scriptblock]$draw) {
  $bmp = New-Object System.Drawing.Bitmap 1400,800
  $g = [System.Drawing.Graphics]::FromImage($bmp); $g.SmoothingMode = 'AntiAlias'; $g.Clear([System.Drawing.Color]::White)
  & $draw $g
  $bmp.Save((Join-Path $out $name), [System.Drawing.Imaging.ImageFormat]::Png)
  $g.Dispose(); $bmp.Dispose()
}
function Text($g,$s,$x,$y,$size=24,$color='Black') { $f=New-Object System.Drawing.Font 'Arial',$size; $b=New-Object System.Drawing.SolidBrush ([System.Drawing.Color]::$color); $g.DrawString($s,$f,$b,$x,$y); $f.Dispose();$b.Dispose() }
function Box($g,$x,$y,$w,$h,$label,$color='LightBlue') { $b=New-Object System.Drawing.SolidBrush ([System.Drawing.Color]::$color);$g.FillRectangle($b,$x,$y,$w,$h);$g.DrawRectangle([System.Drawing.Pens]::Black,$x,$y,$w,$h);Text $g $label ($x+15) ($y+$h/2-15) 22;$b.Dispose() }
Canvas 'stencil_5_point.png' {
 param($g); Text $g 'Five-point Jacobi stencil' 480 35 34
 $cx=650;$cy=365; foreach($p in @(@($cx,($cy-145),'North'),@(($cx-185),$cy,'West'),@($cx,$cy,'Center'),@(($cx+185),$cy,'East'),@($cx,($cy+145),'South'))){Box $g ($p[0] - 65) ($p[1] - 50) 130 100 $p[2] 'LightCyan'}
 Text $g 'u(new) = [North + South + West + East] / 4' 385 650 30 'DarkBlue'; Text $g 'Fixed physical boundaries = 0' 485 710 22 'DarkRed'
}
Canvas 'decomposition_1d_2d.png' {
 param($g); Text $g 'MPI domain decomposition' 500 30 34; Text $g '1D horizontal strips' 170 105 27; Text $g '2D Cartesian blocks' 910 105 27
 for($i=0;$i -lt 4;$i++){Box $g 130 (160+$i*115) 380 100 "Rank $i (owned rows)" ($(if($i%2){'LightGreen'}else{'LightBlue'}))}
 for($r=0;$r -lt 2;$r++){for($c=0;$c -lt 2;$c++){Box $g (850+$c*210) (190+$r*190) 190 170 "Rank $($r*2+$c)" ($(if(($r+$c)%2){'Moccasin'}else{'LightCyan'}))}}
 Text $g 'Halo rows: north / south' 170 670 22 'DarkBlue'; Text $g 'Halo rows + vector datatype columns' 810 670 22 'DarkBlue'
}
Canvas 'overlap_workflow.png' {
 param($g); Text $g 'Communication / computation overlap' 395 35 34
 $labels=@('Post MPI_Irecv','Post MPI_Isend','Compute interior','MPI_Waitall','Compute edges','MPI_Allreduce residual')
 for($i=0;$i -lt $labels.Count;$i++){Box $g (55+$i*220) 290 190 115 $labels[$i] ($(if($i -eq 2){'LightGreen'}elseif($i -eq 4){'Moccasin'}else{'LightBlue'}));if($i -lt $labels.Count-1){$g.DrawLine([System.Drawing.Pens]::DarkBlue,(245+$i*220),347,(275+$i*220),347)}}
 Text $g 'Interior cells are computed while halo messages are in flight.' 300 570 28 'DarkBlue';Text $g 'This is a methodology diagram — not a performance result.' 350 650 22 'DarkRed'
}
Canvas 'surface_to_volume_ratio.png' {
 param($g); Text $g 'Theoretical communication surface-to-volume ratio' 250 30 32
 $one=@(1,2,4,8);$two=@(1,4,9,16);$nx=256;$ny=256
 $axis=New-Object System.Drawing.Pen ([System.Drawing.Color]::Black),3;$g.DrawLine($axis,140,650,1250,650);$g.DrawLine($axis,140,150,140,650)
 $max=.06; $p1=New-Object System.Drawing.Pen ([System.Drawing.Color]::RoyalBlue),4;$p2=New-Object System.Drawing.Pen ([System.Drawing.Color]::DarkOrange),4
 for($i=0;$i -lt $one.Count;$i++){ $v=2*(($one[$i]-1)*$ny)/($nx*$ny);$x=230+$i*250;$y=650-$v/$max*430;$g.FillEllipse([System.Drawing.Brushes]::RoyalBlue,$x-7,$y-7,14,14);if($i){$g.DrawLine($p1,$lx,$ly,$x,$y)};Text $g "$($one[$i])" ($x-8) 670 18;$lx=$x;$ly=$y }
 for($i=0;$i -lt $two.Count;$i++){ $s=[math]::Sqrt($two[$i]);$v=2*((($s-1)*$ny)+(($s-1)*$nx))/($nx*$ny);$x=230+$i*250;$y=650-$v/$max*430;$g.FillRectangle([System.Drawing.Brushes]::DarkOrange,$x-7,$y-7,14,14);if($i){$g.DrawLine($p2,$lx2,$ly2,$x,$y)};Text $g "$($two[$i])" ($x-8) 710 18;$lx2=$x;$ly2=$y }
 Text $g 'Blue circles: 1D strips (P=1,2,4,8)' 170 95 20 'RoyalBlue';Text $g 'Orange squares: 2D square grids (P=1,4,9,16)' 690 95 20 'DarkOrange';Text $g 'Global grid: 256 x 256. Formula: 2[(Pr-1)Ny+(Pc-1)Nx]/(NxNy).' 220 750 20 'DarkRed'
}
Canvas 'synthetic_temperature_heatmap.png' {
 param($g); Text $g 'Synthetic two-hot-spot temperature input (256 x 256)' 270 25 30
 $lines=Get-Content 'dataset/synthetic_temperature_256x256.csv';$min=0;$max=0;foreach($line in $lines){foreach($v in $line.Split(',')){if([double]$v -gt $max){$max=[double]$v}}}
 for($i=0;$i -lt 256;$i++){ $a=$lines[$i].Split(',');for($j=0;$j -lt 256;$j++){ $t=[double]$a[$j]/$max;$r=[int](255*$t);$b=[int](255*(1-$t));$color=[System.Drawing.Color]::FromArgb($r,0,$b);$brush=New-Object System.Drawing.SolidBrush $color;$g.FillRectangle($brush,300+$j*3,105+$i*2,3,2);$brush.Dispose() }}
 Text $g 'SYNTHETIC dataset — arbitrary temperature units; physical boundaries fixed to zero.' 180 670 21 'DarkRed'
}
Canvas 'weak_scaling_problem_sizes.png' {
 param($g); Text $g 'Weak-scaling workload plan (not measured timing)' 305 30 32
 $axis=New-Object System.Drawing.Pen ([System.Drawing.Color]::Black),3;$g.DrawLine($axis,150,650,1230,650);$g.DrawLine($axis,150,120,150,650)
 $p=@(1,4,9,16);$side=@(128,256,384,512);for($i=0;$i -lt 4;$i++){$x=245+$i*250;$h=$side[$i]/512*430;$g.FillRectangle([System.Drawing.Brushes]::SteelBlue,$x,650-$h,150,$h);Text $g "$($side[$i]) x $($side[$i])" ($x-8) (660) 17;Text $g "$($p[$i]) ranks" ($x+25) 700 17};Text $g '2D configuration: approximately 128 x 128 owned cells per process.' 280 755 21 'DarkBlue'
}
function PendingGraph($name,$title,$x,$y,$note) {
  Canvas $name {
    param($g); Text $g $title 330 35 32
    $pen=New-Object System.Drawing.Pen ([System.Drawing.Color]::Black),3;$g.DrawLine($pen,170,650,1240,650);$g.DrawLine($pen,170,140,170,650)
    Text $g $x 650 690 22; Text $g $y 22 360 22
    $font=[System.Drawing.Font]::new('Arial',31,[System.Drawing.FontStyle]::Bold);$brush=New-Object System.Drawing.SolidBrush ([System.Drawing.Color]::DarkRed)
    $g.DrawString('PENDING ACTUAL MPI MEASUREMENTS',$font,$brush,315,335);$font.Dispose();$brush.Dispose()
    Text $g $note 280 410 20 'DarkBlue';Text $g 'Run scripts/run_all.sh, then python3 analysis/plot_results.py.' 290 745 19 'DarkRed'
  }
}
PendingGraph 'strong_scaling.png' 'Strong-scaling execution time' 'Number of MPI processes' 'Execution time (seconds)' 'Fixed global grid; plot median of repeated measured runs.'
PendingGraph 'overlap_speedup.png' 'Overlap versus blocking speedup' 'Number of MPI processes' 'Blocking time / overlap time' 'Plot 1D and 2D series using identical fixed-iteration runs.'
PendingGraph 'decomposition_comparison.png' '1-D versus 2-D decomposition comparison' 'Number of MPI processes' 'Execution time (seconds)' 'Compare 1D and 2D on the same global grid and process count.'
PendingGraph 'weak_scaling.png' 'Weak-scaling performance' 'Number of MPI processes' 'Execution time (seconds)' 'Keep approximately 128 x 128 owned cells per process.'
PendingGraph 'reduction_interval_cost.png' 'Residual-check interval overhead' 'Check interval K' 'Time (seconds)' 'Compare total execution and MPI_Allreduce time for K=1,2,5,10,20.'
