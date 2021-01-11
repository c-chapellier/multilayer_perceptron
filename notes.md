# Plot a graph of results
### In gnuplot:
   set pointsize 3
   plot "< awk '{if($3 == \"red\") print}' new_df.csv" u 1:2 t "red" w p pt 2, "< awk '{if($3 == \"green\") print}' new_df.csv" u 1:2 t "green" w p pt 2, "< awk '{if($3 == \"blue\") print}' new_df.csv" u 1:2 t "blue" w p pt 2