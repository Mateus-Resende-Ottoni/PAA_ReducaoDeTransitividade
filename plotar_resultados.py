import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Carregar os dados gerados pelo C++
df = pd.read_csv('Resultados/resultados_paa.csv')

sns.set_theme(style="whitegrid", palette="colorblind")
plt.rcParams.update({'font.size': 12})

# Criar a grade dividida por densidade e desenhar as curvas
g = sns.FacetGrid(df, col="Densidade", height=5, aspect=1.2, sharey=False)

g.map_dataframe(sns.lineplot, x="V", y="TempoMedio_us", hue="Algoritmo", 
                marker="o", linewidth=2, markersize=6)

# Ativar escala logarítmica no eixo Y para cada subplot
for ax in g.axes.flat:
    ax.set_yscale('log') # Transforma o colapso de escala em curvas legíveis
    ax.set_ylabel("Tempo Médio (µs) - Escala Log")

g.set_axis_labels("Número de Vértices (|V|)", "Tempo de Execução")
g.set_titles("Topologia DAG: {col_name}")
g.add_legend(title="Algoritmo")

plt.savefig('Resultados/analise_assintotica_reducao.png', dpi=300, bbox_inches='tight')
plt.show()