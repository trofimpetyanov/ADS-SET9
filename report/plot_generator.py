import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import os

def generate_plots(csv_file, output_dir):
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    try:
        data = pd.read_csv(csv_file)
    except FileNotFoundError:
        print(f"Error: The file {csv_file} was not found.")
        return
    except pd.errors.EmptyDataError:
        print(f"Error: The file {csv_file} is empty.")
        return
    except Exception as e:
        print(f"An error occurred while reading {csv_file}: {e}")
        return

    data_types = data['DataType'].unique()
    algorithms = data['Algorithm'].unique()

    metrics = ['Time_ms', 'CharComparisons']
    metric_titles = {
        'Time_ms': 'Execution Time (ms)',
        'CharComparisons': 'Character Comparisons'
    }
    y_labels = {
        'Time_ms': 'Time (ms)',
        'CharComparisons': 'Comparisons'
    }

    for dt in data_types:
        dt_data = data[data['DataType'] == dt]
        if dt_data.empty:
            print(f"No data for data type: {dt}. Skipping plots for this type.")
            continue

        for metric in metrics:
            plt.figure(figsize=(12, 8))
            for algo in algorithms:
                algo_data = dt_data[dt_data['Algorithm'] == algo]
                if not algo_data.empty:
                    sns.lineplot(x='ArraySize', y=metric, data=algo_data, label=algo, marker='o')
            
            plt.title(f'{metric_titles[metric]} vs. Array Size for {dt} Data')
            plt.xlabel('Array Size')
            plt.ylabel(y_labels[metric])
            plt.legend(title='Algorithm', bbox_to_anchor=(1.05, 1), loc='upper left')
            plt.grid(True)
            plt.tight_layout(rect=[0, 0, 0.85, 1]) 
            
            plot_filename = os.path.join(output_dir, f'{dt}_{metric}.png')
            try:
                plt.savefig(plot_filename)
                print(f"Saved plot: {plot_filename}")
            except Exception as e:
                print(f"Error saving plot {plot_filename}: {e}")
            plt.close()

if __name__ == '__main__':
    generate_plots('sort_experiment_results.csv', 'plots')
    print("Plot generation finished.") 