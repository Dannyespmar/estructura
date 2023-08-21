import tkinter as tk
import random
import networkx as nx
import time
from tkinter import messagebox

def kruskal(graph):
    mst = nx.minimum_spanning_tree(graph)
    return mst.edges()

def prim(graph):
    mst = nx.minimum_spanning_tree(graph, algorithm="prim")
    return mst.edges()

def generar_grafo_aleatorio(n, p):
    graph = nx.erdos_renyi_graph(n, p)
    for u, v in graph.edges():
        graph.edges[u, v]["weight"] = random.randint(1, 1000)
    return graph

def ejecutar_y_mostrar_resultados(algoritmo):
    try:
        n = int(nodos_var.get())
        p = float(probabilidad_var.get())
        graph = generar_grafo_aleatorio(n, p)
        
        tiempo_inicio = time.time()
        if algoritmo == "Kruskal":
            mst_edges = kruskal(graph)
        elif algoritmo == "Prim":
            mst_edges = prim(graph)
        tiempo_transcurrido = time.time() - tiempo_inicio

        mst_weight = sum(graph[u][v]["weight"] for u, v in mst_edges)
        resultado_var.set(f"Peso del Árbol de Expansión Mínima: {mst_weight}")
        tiempo_var.set(f"Tiempo Transcurrido: {tiempo_transcurrido:.6f} segundos")
    except Exception as e:
        messagebox.showerror("Error", str(e))

root = tk.Tk()
root.title("Aplicación de Árbol de Expansión Mínima")

algoritmo_var = tk.StringVar(value="Kruskal")
etiqueta_algoritmo = tk.Label(root, text="Elija el algoritmo:")
etiqueta_algoritmo.pack()

menu_algoritmo = tk.OptionMenu(root, algoritmo_var, "Kruskal", "Prim")
menu_algoritmo.pack()

etiqueta_nodos = tk.Label(root, text="Número de Nodos:")
etiqueta_nodos.pack()

nodos_var = tk.StringVar(value="50")
entrada_nodos = tk.Entry(root, textvariable=nodos_var)
entrada_nodos.pack()

etiqueta_probabilidad = tk.Label(root, text="Probabilidad:")
etiqueta_probabilidad.pack()

probabilidad_var = tk.StringVar(value="0.3")
entrada_probabilidad = tk.Entry(root, textvariable=probabilidad_var)
entrada_probabilidad.pack()

boton_ejecutar = tk.Button(root, text="Ejecutar Algoritmo", command=lambda: ejecutar_y_mostrar_resultados(algoritmo_var.get()))
boton_ejecutar.pack()

resultado_var = tk.StringVar(value="")
etiqueta_resultado = tk.Label(root, textvariable=resultado_var)
etiqueta_resultado.pack()

tiempo_var = tk.StringVar(value="")
etiqueta_tiempo = tk.Label(root, textvariable=tiempo_var)
etiqueta_tiempo.pack()

root.mainloop()

