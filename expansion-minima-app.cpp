#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QMessageBox>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <random>
#include <ctime>
#include <vector>
#include <chrono>
#include <iostream>
#include <algorithm>
#include <QElapsedTimer>
#include <QTime>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

using namespace std;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              boost::no_property, boost::property<boost::edge_weight_t, int>> Graph;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;

Graph generar_grafo_aleatorio(int n, double p) {
    Graph graph(n);
    default_random_engine generator(time(0));
    uniform_int_distribution<int> weight_dist(1, 1000);
    for (int u = 0; u < n; ++u) {
        for (int v = u + 1; v < n; ++v) {
            if (static_cast<double>(rand()) / RAND_MAX < p) {
                Edge e;
                bool inserted;
                tie(e, inserted) = boost::add_edge(u, v, graph);
                if (inserted) {
                    boost::put(boost::edge_weight, graph, e, weight_dist(generator));
                }
            }
        }
    }
    return graph;
}

vector<Edge> kruskal(Graph &graph) {
    vector<Edge> mst;
    boost::kruskal_minimum_spanning_tree(graph, back_inserter(mst));
    return mst;
}

vector<Edge> prim(Graph &graph) {
    vector<Edge> mst;
    boost::prim_minimum_spanning_tree(graph, back_inserter(mst));
    return mst;
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget *window = new QWidget;
    window->setWindowTitle("Aplicación de Árbol de Expansión Mínima");

    QLabel *etiqueta_algoritmo = new QLabel("Elija el algoritmo:");
    QComboBox *menu_algoritmo = new QComboBox;
    menu_algoritmo->addItem("Kruskal");
    menu_algoritmo->addItem("Prim");

    QLabel *etiqueta_nodos = new QLabel("Número de Nodos:");
    QLineEdit *entrada_nodos = new QLineEdit("50");

    QLabel *etiqueta_probabilidad = new QLabel("Probabilidad:");
    QLineEdit *entrada_probabilidad = new QLineEdit("0.3");

    QPushButton *boton_ejecutar = new QPushButton("Ejecutar Algoritmo");

    QLabel *etiqueta_resultado = new QLabel;
    QLabel *etiqueta_tiempo = new QLabel;

    QFormLayout *layout_form = new QFormLayout;
    layout_form->addRow(etiqueta_algoritmo, menu_algoritmo);
    layout_form->addRow(etiqueta_nodos, entrada_nodos);
    layout_form->addRow(etiqueta_probabilidad, entrada_probabilidad);
    layout_form->addWidget(boton_ejecutar);
    layout_form->addRow("Resultado:", etiqueta_resultado);
    layout_form->addRow("Tiempo:", etiqueta_tiempo);

    window->setLayout(layout_form);
    window->show();

    QObject::connect(boton_ejecutar, &QPushButton::clicked, [&] {
        QString algoritmo = menu_algoritmo->currentText();
        int n = entrada_nodos->text().toInt();
        double p = entrada_probabilidad->text().toDouble();
        Graph graph = generar_grafo_aleatorio(n, p);

        QElapsedTimer timer;
        timer.start();

        vector<Edge> mst_edges;
        if (algoritmo == "Kruskal") {
            mst_edges = kruskal(graph);
        } else if (algoritmo == "Prim") {
            mst_edges = prim(graph);
        }

        double tiempo_transcurrido = static_cast<double>(timer.elapsed()) / 1000.0;

        int mst_weight = 0;
        for (const Edge &e : mst_edges) {
            mst_weight += boost::get(boost::edge_weight, graph, e);
        }

        etiqueta_resultado->setText(QString("Peso del Árbol de Expansión Mínima: %1").arg(mst_weight));
        etiqueta_tiempo->setText(QString("Tiempo Transcurrido: %1 segundos").arg(tiempo_transcurrido));
    });

    return app.exec();
}
