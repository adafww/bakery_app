#include "MainWindow.h"
#include "DatabaseManager.h"
#include <QVBoxLayout>
#include <QTabWidget>
#include <QPushButton>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    DatabaseManager dbManager;
    tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);

    tabWidget->addTab(createTableView("products"), "Продукты");
    tabWidget->addTab(createTableView("orders"), "Заказы");
    tabWidget->addTab(createTableView("employees"), "Сотрудники");
    tabWidget->addTab(createTableView("production"), "Производство");
    tabWidget->addTab(createTableView("sales"), "Продажи");

    QVBoxLayout *layout = new QVBoxLayout;
    QPushButton *refreshButton = new QPushButton("Обновить", this);
    QPushButton *addButton = new QPushButton("Добавить", this);
    QPushButton *deleteButton = new QPushButton("Удалить", this);

    layout->addWidget(refreshButton);
    layout->addWidget(addButton);
    layout->addWidget(deleteButton);

    QWidget *widget = new QWidget();
    widget->setLayout(layout);
    setMenuWidget(widget);

    connect(refreshButton, &QPushButton::clicked, this, &MainWindow::refreshData);
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addRecord);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::deleteRecord);
    
    setWindowTitle("Учёт работы хлебозавода");
    setStyleSheet(
        "QMainWindow { background-color: #2B2B2B; }"  // Фон главного окна
        "QTabWidget::pane { background-color: #2B2B2B; }" // Фон вкладок
        "QTabBar::tab { background: #3B3B3B; color: white; padding: 5px; }"
        "QTabBar::tab:selected { background: #4B4B4B; }"

        // === Стили для таблицы ===
        "QTableView { background-color: #3B3B3B; color: white; gridline-color: gray; border: none; }" // Фон таблицы
        "QTableView::item { background-color: #3B3B3B; color: white; }" // Ячейки таблицы
        "QTableView::item:selected { background-color: #555555; color: white; }" // Выделенные строки
        "QTableView QScrollBar:vertical { background: #2B2B2B; width: 10px; }" // Вертикальная полоса прокрутки
        "QTableView QScrollBar:horizontal { background: #2B2B2B; height: 10px; }" // Горизонтальная полоса прокрутки
        "QTableView QScrollBar::handle { background: #555555; }" // Ползунок прокрутки

        // === Заголовки столбцов и строк ===
        "QHeaderView::section { background-color: #4B4B4B; color: white; padding: 5px; border: 1px solid gray; }"
        "QTableCornerButton::section { background-color: #4B4B4B; border: 1px solid gray; }"
    
        // **Исправление белого фона вокруг заголовков**
        "QHeaderView { background-color: #4B4B4B; border: none; }"  // Фон вокруг заголовков строк и столбцов
        "QTableView::viewport { background-color: #3B3B3B; }"  // Фон за пределами таблицы
    
        // === Кнопки ===
        "QPushButton { background-color: #4B4B4B; color: white; border-radius: 5px; padding: 5px; }"
        "QPushButton:hover { background-color: #5C5C5C; }"
    );
    resize(800, 600);
}

MainWindow::~MainWindow() {}

QTableView* MainWindow::createTableView(const QString &tableName) {
    QSqlTableModel *model = new QSqlTableModel(this);
    model->setTable(tableName);
    model->setEditStrategy(QSqlTableModel::OnRowChange);
    model->select();

    if (!model->select()) {
        qDebug() << "Ошибка при загрузке данных для таблицы:" << tableName;
    }

    QTableView *view = new QTableView;
    view->setModel(model);
    view->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    return view;
}

void MainWindow::refreshData() {
    QTableView *currentView = qobject_cast<QTableView*>(tabWidget->currentWidget());
    QSqlTableModel *model = qobject_cast<QSqlTableModel*>(currentView->model());

    if (model) {
        model->select();
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось обновить данные.");
    }
}

void MainWindow::addRecord() {
    QTableView *currentView = qobject_cast<QTableView*>(tabWidget->currentWidget());
    QSqlTableModel *model = qobject_cast<QSqlTableModel*>(currentView->model());

    if (model) {
        model->insertRow(model->rowCount());
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось добавить запись.");
    }
}

void MainWindow::deleteRecord() {
    QTableView *currentView = qobject_cast<QTableView*>(tabWidget->currentWidget());
    QSqlTableModel *model = qobject_cast<QSqlTableModel*>(currentView->model());

    if (model) {
        int row = currentView->currentIndex().row();
        if (row >= 0) {
            model->removeRow(row);
            model->submitAll();
        } else {
            QMessageBox::warning(this, "Ошибка", "Выберите запись для удаления.");
        }
    }
}
