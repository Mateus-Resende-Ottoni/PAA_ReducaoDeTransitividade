import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Carregar os dados gerados pelo C++
df = pd.read_csv('Resultados/resultados_paa.csv')

sns.set_theme(style="whitegrid", palette="colorblind")
plt.rcParams.update({'font.size': 12})

# Obter as densidades únicas presentes nos dados
densidades = df['Densidade'].unique()

# Gerar e salvar um gráfico para cada densidade
for densidade in densidades:
    plt.figure(figsize=(7, 5))
    
    # Filtrar os dados apenas para a densidade atual
    df_plot = df[df['Densidade'] == densidade]
    
    # Desenhar as curvas
    sns.lineplot(data=df_plot, x="V", y="TempoMedio_us", hue="Algoritmo", 
                 marker="o", linewidth=2, markersize=6)
    
    # Configurações dos eixos e título
    plt.yscale('log')
    plt.ylabel("Tempo Médio (µs) - Escala Log")
    plt.xlabel("Número de Vértices (|V|)")
    plt.title(f"Topologia DAG: {densidade}")
    
    # Posicionar a legenda fora do gráfico (opcional, mas recomendado para clareza)
    plt.legend(title="Algoritmo", bbox_to_anchor=(1.05, 1), loc='upper left')
    
    # Gerar nome do arquivo dinamicamente (ex: grafico_esparso.png)
    nome_arquivo = f'Resultados/grafico_{str(densidade).lower()}.png'
    
    # Salvar a imagem
    plt.savefig(nome_arquivo, dpi=300, bbox_inches='tight')
    
    # Limpar a figura atual para a próxima iteração
    plt.clf()
    plt.close()