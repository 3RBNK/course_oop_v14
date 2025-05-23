#include "../include/admin_interface.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QDateTime>

AdminInterface::AdminInterface(User *admin, QWidget *parent)
        : Interface(parent), m_admin(admin) {
    if (!admin || admin->role() != "admin") {
        QMessageBox::critical(this, "Ошибка", "Недопустимый пользователь-администратор");
        throw std::invalid_argument("Invalid admin user");
    }
    m_auth_system = new AuthSystem(this);
    m_schedule = new Schedule(this);
    setup_ui();
    refresh_users_table();
    refresh_schedule_table();
}

AdminInterface::~AdminInterface() {
    delete m_auth_system;
    delete m_schedule;
}

void AdminInterface::setup_ui() {
    auto *main_layout = new QVBoxLayout(this);
    tab_widget = new QTabWidget(this);

    users_tab = new QWidget();
    schedule_tab = new QWidget();

    setup_users_tab();
    setup_schedule_tab();

    tab_widget->addTab(users_tab, "Управление пользователями");
    tab_widget->addTab(schedule_tab, "Управление расписанием");

    main_layout->addWidget(tab_widget);
    setLayout(main_layout);
    setWindowTitle("Панель администратора");
    resize(800, 600);
}

void AdminInterface::setup_users_tab() {
    auto *layout = new QVBoxLayout(users_tab);

    users_table = new QTableWidget(0, 5, users_tab);
    users_table->setHorizontalHeaderLabels({"ID", "Имя", "Роль", "Логин", "Пароль"});
    users_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    auto *form_layout = new QHBoxLayout();
    name_edit = new QLineEdit();
    name_edit->setPlaceholderText("Имя");
    login_edit = new QLineEdit();
    login_edit->setPlaceholderText("Логин");
    password_edit = new QLineEdit();
    password_edit->setPlaceholderText("Пароль");
    role_combo = new QComboBox();
    populate_role_combo();

    auto *add_btn = new QPushButton("Добавить пользователя");
    auto *delete_btn = new QPushButton("Удалить пользователя");

    form_layout->addWidget(name_edit);
    form_layout->addWidget(login_edit);
    form_layout->addWidget(password_edit);
    form_layout->addWidget(role_combo);
    form_layout->addWidget(add_btn);
    form_layout->addWidget(delete_btn);

    layout->addWidget(users_table);
    layout->addLayout(form_layout);

    connect(add_btn, &QPushButton::clicked, this, &AdminInterface::add_user);
    connect(delete_btn, &QPushButton::clicked, this, &AdminInterface::delete_user);
}

void AdminInterface::setup_schedule_tab() {
    auto *layout = new QVBoxLayout(schedule_tab);

    schedule_table = new QTableWidget(6, 6);
    QStringList days = {"Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота"};
    QStringList times = {"8:15-9:50", "10:00-11:35", "11:45-13:20",
                         "14:20-15:55", "16:05-17:40", "17:50-19:25"};
    schedule_table->setHorizontalHeaderLabels(days);
    schedule_table->setVerticalHeaderLabels(times);

    auto *form_layout = new QHBoxLayout();
    user_combo = new QComboBox();
    subject_edit = new QLineEdit();
    subject_edit->setPlaceholderText("Предмет");
    day_combo = new QComboBox();
    day_combo->addItems(days);
    time_combo = new QComboBox();
    time_combo->addItems(times);
    auditorium_edit = new QLineEdit();
    auditorium_edit->setPlaceholderText("Аудитория");
    teacher_edit = new QLineEdit();
    teacher_edit->setPlaceholderText("Преподаватель");

    auto *add_lesson_btn = new QPushButton("Добавить занятие");
    auto *delete_lesson_btn = new QPushButton("Удалить занятие");

    form_layout->addWidget(user_combo);
    form_layout->addWidget(subject_edit);
    form_layout->addWidget(day_combo);
    form_layout->addWidget(time_combo);
    form_layout->addWidget(auditorium_edit);
    form_layout->addWidget(teacher_edit);
    form_layout->addWidget(add_lesson_btn);
    form_layout->addWidget(delete_lesson_btn);

    layout->addWidget(schedule_table);
    layout->addLayout(form_layout);

    connect(add_lesson_btn, &QPushButton::clicked, this, &AdminInterface::add_lesson);
    connect(delete_lesson_btn, &QPushButton::clicked, this, &AdminInterface::delete_lesson);
    connect(user_combo, &QComboBox::currentTextChanged, this, &AdminInterface::on_user_selected);
}

void AdminInterface::populate_role_combo() {
    role_combo->addItems({"student", "teacher", "admin"});
}

void AdminInterface::add_user() {
    QString name = name_edit->text();
    QString login = login_edit->text();
    QString password = password_edit->text();
    QString role = role_combo->currentText();

    if (name.isEmpty() || login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Все поля должны быть заполнены");
        return;
    }

    int id;
    if (role == "admin") {
        id = 2000 + m_auth_system->m_users.size();
    } else if (role == "teacher") {
        id = 1000 + m_auth_system->m_users.size();
    } else {
        id = m_auth_system->m_users.size();
    }

    auto *new_user = new User(id, name, role, login, password);
    m_auth_system->add_user(new_user);
    refresh_users_table();

    name_edit->clear();
    login_edit->clear();
    password_edit->clear();
}

void AdminInterface::delete_user() {
    auto selected_items = users_table->selectedItems();
    if (selected_items.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Выберите пользователя для удаления");
        return;
    }

    int row = selected_items[0]->row();
    int user_id = users_table->item(row, 0)->text().toInt();

    for (int i = 0; i < m_auth_system->m_users.size(); ++i) {
        if (m_auth_system->m_users[i]->user_id() == user_id) {
            delete m_auth_system->m_users[i];
            m_auth_system->m_users.removeAt(i);
            break;
        }
    }

    refresh_users_table();
}

void AdminInterface::add_lesson() {
    QString selected_user_name = user_combo->currentText();
    if (selected_user_name.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Выберите пользователя");
        return;
    }

    User *selected_user = nullptr;
    for (auto *user: m_auth_system->m_users) {
        if (user->name() == selected_user_name) {
            selected_user = user;
            break;
        }
    }

    if (!selected_user) {
        QMessageBox::warning(this, "Ошибка", "Пользователь не найден");
        return;
    }

    QString subject = subject_edit->text();
    QString auditorium_name = auditorium_edit->text();
    QString teacher_name = teacher_edit->text();

    if (subject.isEmpty() || auditorium_name.isEmpty() || teacher_name.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Все поля должны быть заполнены");
        return;
    }

    int day_index = day_combo->currentIndex() + 1;
    QString time_str = time_combo->currentText();
    QStringList time_parts = time_str.split("-")[0].split(":");

    QDate current_date = QDate::currentDate();
    while (current_date.dayOfWeek() != day_index) {
        current_date = current_date.addDays(1);
    }

    QTime lesson_time = QTime::fromString(time_parts[0], "HH:mm");
    if (!lesson_time.isValid()) {
        QMessageBox::warning(this, "Ошибка", "Неверный формат времени");
        return;
    }
    QDateTime lesson_datetime(current_date, lesson_time);

    Auditorium auditorium(auditorium_name);
    Teacher teacher(teacher_name);

    for (const auto &lesson: m_schedule->lessons()) {
        if (lesson->time_slot().time() == lesson_time &&
            lesson->time_slot().date().dayOfWeek() == day_index) {
            QMessageBox::warning(this, "Ошибка",
                                 "В это время уже есть занятие в расписании");
            return;
        }
    }

    auto lesson = QSharedPointer<Lesson>::create(
            0,
            subject,
            teacher,
            QList < Group > (),
            lesson_datetime,
            auditorium
    );

    m_schedule->load_from_json("D:\\home_work\\oop\\course\\code_v1\\users\\users.json", *selected_user);
    m_schedule->add_lesson(lesson);
    m_schedule->save_to_json("D:\\home_work\\oop\\course\\code_v1\\users\\users.json", *selected_user);

    refresh_schedule_table();

    subject_edit->clear();
    auditorium_edit->clear();
    teacher_edit->clear();
}

void AdminInterface::delete_lesson() {
    QString selected_user_name = user_combo->currentText();
    if (selected_user_name.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Выберите пользователя");
        return;
    }

    auto selected_items = schedule_table->selectedItems();
    if (selected_items.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Выберите занятие для удаления");
        return;
    }

    User *selected_user = nullptr;
    for (auto *user: m_auth_system->m_users) {
        if (user->name() == selected_user_name) {
            selected_user = user;
            break;
        }
    }

    if (!selected_user) {
        QMessageBox::warning(this, "Ошибка", "Пользователь не найден");
        return;
    }

    int row = selected_items[0]->row();
    int column = selected_items[0]->column();

    QStringList times = {"8:15", "10:00", "11:45", "14:20", "16:05", "17:50"};
    QTime lesson_time = QTime::fromString(times[row], "HH:mm");
    int day_of_week = column + 1;

    m_schedule->load_from_json("D:\\home_work\\oop\\course\\code_v1\\users\\users.json", *selected_user);

    QList < QSharedPointer < Lesson >> lessons = m_schedule->lessons();
    for (int i = 0; i < lessons.size(); ++i) {
        const auto &lesson = lessons[i];
        if (lesson->time_slot().time() == lesson_time &&
            lesson->time_slot().date().dayOfWeek() == day_of_week) {
            m_schedule->remove_lesson(i);
            break;
        }
    }

    m_schedule->save_to_json("D:\\home_work\\oop\\course\\code_v1\\users\\users.json", *selected_user);
    refresh_schedule_table();
}

void AdminInterface::refresh_users_table() {
    users_table->setRowCount(0);
    for (const auto *user: m_auth_system->m_users) {
        int row = users_table->rowCount();
        users_table->insertRow(row);
        users_table->setItem(row, 0, new QTableWidgetItem(QString::number(user->user_id())));
        users_table->setItem(row, 1, new QTableWidgetItem(user->name()));
        users_table->setItem(row, 2, new QTableWidgetItem(user->role()));
        users_table->setItem(row, 3, new QTableWidgetItem(user->login()));
        users_table->setItem(row, 4, new QTableWidgetItem(user->password()));
    }
}

void AdminInterface::refresh_schedule_table() {
    for (int i = 0; i < schedule_table->rowCount(); ++i) {
        for (int j = 0; j < schedule_table->columnCount(); ++j) {
            if (schedule_table->item(i, j)) {
                delete schedule_table->item(i, j);
            }
        }
    }

    QString selected_user_name = user_combo->currentText();
    if (selected_user_name.isEmpty()) {
        return;
    }

    User *selected_user = nullptr;
    for (auto *user: m_auth_system->m_users) {
        if (user->name() == selected_user_name) {
            selected_user = user;
            break;
        }
    }

    if (!selected_user) {
        return;
    }

    m_schedule->load_from_json("D:\\home_work\\oop\\course\\code_v1\\users\\users.json", *selected_user);

    for (const auto &lesson: m_schedule->lessons()) {
        QDateTime time_slot = lesson->time_slot();
        int day = time_slot.date().dayOfWeek() - 1;

        QTime lesson_time = time_slot.time();
        int row = -1;
        if (lesson_time >= QTime(8, 15) && lesson_time < QTime(9, 50)) row = 0;
        else if (lesson_time >= QTime(10, 0) && lesson_time < QTime(11, 35)) row = 1;
        else if (lesson_time >= QTime(11, 45) && lesson_time < QTime(13, 20)) row = 2;
        else if (lesson_time >= QTime(14, 20) && lesson_time < QTime(15, 55)) row = 3;
        else if (lesson_time >= QTime(16, 5) && lesson_time < QTime(17, 40)) row = 4;
        else if (lesson_time >= QTime(17, 50) && lesson_time < QTime(19, 25)) row = 5;

        if (row != -1) {
            QString text = lesson->subject() + "\n"
                           + lesson->auditorium().auditorium_info() + "\n"
                           + lesson->teacher().teacher_info();

            auto *item = new QTableWidgetItem(text);
            item->setTextAlignment(Qt::AlignCenter);
            schedule_table->setItem(row, day, item);
        }
    }
}

void AdminInterface::on_user_selected() {
    refresh_schedule_table();
}

void AdminInterface::display() {
    show();
}