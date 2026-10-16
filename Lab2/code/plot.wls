data = Rest[Import["benchmark_results.csv", "CSV"]];
labels = data[[All, 1]];
values = ToExpression[data[[All, 2]]];

chart = BarChart[values,
  ChartLabels -> Placed[labels, Axis],
  ScalingFunctions -> "Log",
  ChartStyle -> "Pastel",
  PlotTheme -> "Detailed",
  FrameLabel -> {"Operation", "Time (ns, Log Scale)"},
  PlotLabel -> "BigInt Modular Arithmetic Benchmarks",
  ImageSize -> Large
];

Export["benchmark_chart.png", chart];