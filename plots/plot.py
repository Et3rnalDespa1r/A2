import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path

df = pd.read_csv("../results_a2.csv")

types = ["random", "reversed", "almost"]
rus_titles = {
    "random": "Random",
    "reversed": "Reversed",
    "almost": "Almost",
}

for arr_type in types:
    sub = df[df["тип_массива"] == arr_type]
    if sub.empty:
        continue

    plt.figure(figsize=(10, 6))

    merge_rows = sub[sub["алгоритм"] == "merge"]
    plt.plot(
        merge_rows["размер"],
        merge_rows["время_мкс"],
        label="merge",
        linewidth=2,
    )

    hybrid_rows = sub[sub["алгоритм"] == "hybrid"]
    for thr in sorted(hybrid_rows["threshold"].unique()):
        thr_rows = hybrid_rows[hybrid_rows["threshold"] == thr]
        plt.plot(
            thr_rows["размер"],
            thr_rows["время_мкс"],
            label=f"hybrid (t={thr})",
            linewidth=1,
            alpha=0.8,
        )

    plt.title(f"{rus_titles[arr_type]} — время от размера n")
    plt.xlabel("Размер массива, n")
    plt.ylabel("Время, мкс")
    plt.grid(True, alpha=0.3)
    plt.legend()
    plt.tight_layout()

    out_path = Path(__file__).resolve().parent / f"{arr_type}_times.png"
    plt.savefig(out_path, dpi=200)
    plt.close()

for arr_type in types:
    sub = df[(df["тип_массива"] == arr_type) & (df["алгоритм"] == "hybrid")]
    if sub.empty:
        continue

    grouped = sub.groupby("threshold")["время_мкс"].mean().reset_index()
    plt.figure(figsize=(8, 5))
    plt.plot(
        grouped["threshold"],
        grouped["время_мкс"],
        marker="o",
        linestyle="-",
    )
    plt.xscale("log", base=10)
    plt.title(f"{rus_titles[arr_type]} — влияние порога")
    plt.xlabel("Порог вставки")
    plt.ylabel("Среднее время, мкс")
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    out_path = Path(__file__).resolve().parent / f"{arr_type}_threshold.png"
    plt.savefig(out_path, dpi=200)
    plt.close()
print("Готово: сохранены *times.png и *threshold.png для random/reversed/almost.")
