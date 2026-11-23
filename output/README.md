# Document images

These PNGs are methodology diagrams and valid non-performance graphs generated locally from `scripts/generate_document_images.ps1`. `synthetic_temperature_heatmap.png` visualizes the actual synthetic dataset. `surface_to_volume_ratio.png` is calculated directly from the documented theoretical formula. `weak_scaling_problem_sizes.png` documents the planned workload. They are not benchmark graphs and contain no fabricated measurements. MPI performance graphs can only be created after an Open MPI run produces CSV files in `results/raw/`.

The five `*_pending` performance-figure templates use the final figure names and are explicitly marked as pending actual MPI measurements. They are report-layout templates, not results.
