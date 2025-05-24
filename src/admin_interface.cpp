#include "../include/admin_interface.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QDateTime>
#include <QInputDialog>
#include <QHeaderView>

AdminInterface::AdminInterface(User *admin, QWidget *parent)
        : Interface(parent), m_admin(admin) {
    m_auth_system = new AuthSystem();
    m_schedule = new Schedule();
    setup_ui();
    refresh_users_table();
    populate_user_combo();
    refresh_schedule_table();
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

    auto *buttons_layout = new QHBoxLayout();
    auto *add_btn = new QPushButton("Добавить пользователя");
    auto *delete_btn = new QPushButton("Удалить пользователя");
    auto *save_btn = new QPushButton("Сохранить изменения");

    form_layout->addWidget(name_edit);
    form_layout->addWidget(login_edit);
    form_layout->addWidget(password_edit);
    form_layout->addWidget(role_combo);

    buttons_layout->addWidget(add_btn);
    buttons_layout->addWidget(delete_btn);
    buttons_layout->addWidget(save_btn);

    layout->addWidget(users_table);
    layout->addLayout(form_layout);
    layout->addLayout(buttons_layout);

    connect(add_btn, &QPushButton::clicked, this, &AdminInterface::add_user);
    connect(delete_btn, &QPushButton::clicked, this, &AdminInterface::delete_user);
    connect(save_btn, &QPushButton::clicked, this, &AdminInterface::save_users_to_json);
}

void AdminInterface::setup_schedule_tab() {
    auto *layout = new QVBoxLayout(schedule_tab);

    schedule_table = new QTableWidget(6, 6);
    QStringList days = {"Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота"};
    QStringList times = {"08:15-09:50", "10:00-11:35", "11:45-13:20",
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
    connect(user_combo, &QComboBox::currentTextChanged, this, &AdminInterface::refresh_schedule_table);
}


void AdminInterface::add_user() {
    QString name = name_edit->text();
    QString login = login_edit->text();
    QString password = password_edit->text();
    QString role = role_combo->currentText();

    if (name.isEmpty() || login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Заполните все обязательные поля");
        return;
    }

    int user_id = generate_unique_user_id();

    if (role == "student") {
        bool ok;
        int group_id = QInputDialog::getInt(this, "Группа студента",
                                            "Введите ID группы:", 1, 1, 999999, 1, &ok);
        if (!ok) {
            QMessageBox::warning(this, "Ошибка", "Необходимо указать ID группы");
            return;
        }

        int course = QInputDialog::getInt(this, "Курс студента",
                                          "Введите номер курса:", 1, 1, 6, 1, &ok);
        if (!ok) {
            QMessageBox::warning(this, "Ошибка", "Необходимо указать курс");
            return;
        }

        auto *group = new Group(user_id, name, role, login, password, group_id, course);
        m_auth_system->add_user(group);
    } else if (role == "teacher") {
        bool ok;
        int teacher_id = QInputDialog::getInt(this, "ID преподавателя",
                                              "Введите ID преподавателя:", 1, 1, 999999, 1, &ok);
        if (!ok) {
            QMessageBox::warning(this, "Ошибка", "Необходимо указать ID преподавателя");
            return;
        }

        QStringList subjects;
        while (true) {
            QString subject = QInputDialog::getText(this, "Предмет преподавателя",
                                                    "Введите название предмета (или оставьте пустым для завершения):",
                                                    QLineEdit::Normal, "", &ok);

            if (!ok || subject.isEmpty()) {
                break;
            }
            subjects.append(subject);
        }

        if (subjects.isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Необходимо указать хотя бы один предмет");
            return;
        }

        auto *teacher = new Teacher(user_id, name, role, login, password, teacher_id, subjects);
        m_auth_system->add_user(teacher);
    } else {
        auto *admin = new User(user_id, name, role, login, password);
        m_auth_system->add_user(admin);
    }

    refresh_users_table();
    populate_user_combo();

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
    populate_user_combo();
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
    QString time_parts = time_str.split("-")[0];

    qDebug() << "Time: " << time_parts;

    QDate current_date = QDate::currentDate();
    while (current_date.dayOfWeek() != day_index) {
        current_date = current_date.addDays(1);
    }

    QTime lesson_time = QTime::fromString(time_parts, "HH:mm");
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
            QList<Group>(),
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
    QList<QSharedPointer<Lesson >> lessons = m_schedule->lessons();
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
    int row = 0;

    for (const auto *user: m_auth_system->get_users()) {
        users_table->insertRow(row);

        users_table->setItem(row, 0, new QTableWidgetItem(QString::number(user->user_id())));
        users_table->setItem(row, 1, new QTableWidgetItem(user->name()));
        users_table->setItem(row, 2, new QTableWidgetItem(user->role()));
        users_table->setItem(row, 3, new QTableWidgetItem(user->login()));
        users_table->setItem(row, 4, new QTableWidgetItem(user->password()));

        if (const auto *group = dynamic_cast<const Group *>(user)) {
            users_table->setItem(row, 5, new QTableWidgetItem(QString::number(group->group_id())));
            users_table->setItem(row, 6, new QTableWidgetItem(QString::number(group->course())));
        }

        if (const auto *teacher = dynamic_cast<const Teacher *>(user)) {
            users_table->setItem(row, 7, new QTableWidgetItem(teacher->subjects().join(", ")));
        }

        row++;
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

int AdminInterface::generate_unique_user_id() {
    int max_id = 0;
    for (const auto *user: m_auth_system->get_users()) {
        if (user->user_id() > max_id) {
            max_id = user->user_id();
        }
    }
    return max_id + 1;
}

int AdminInterface::generate_unique_teacher_id() {
    int max_id = 1000;
    for (const auto *user: m_auth_system->get_users()) {
        if (const auto *teacher = dynamic_cast<const Teacher *>(user)) {
            if (teacher->teacher_id() > max_id) {
                max_id = teacher->teacher_id();
            }
        }
    }
    return max_id + 1;
}

void AdminInterface::populate_role_combo() {
    role_combo->clear();
    role_combo->addItems({"student", "teacher", "admin"});
}

void AdminInterface::populate_user_combo() {
    user_combo->clear();
    for (const auto *user: m_auth_system->m_users) {
        if (user->role() == "student" || user->role() == "teacher") {
            user_combo->addItem(user->name());
        }
    }
}

void AdminInterface::on_user_selected() {
    refresh_schedule_table();
}

void AdminInterface::save_users_to_json() {
    try {
        m_auth_system->save_to_json("D:\\home_work\\oop\\course\\code_v1\\users\\users.json");
        QMessageBox::information(this, "Успех", "Пользователи успешно сохранены в файл");
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Ошибка", QString("Не удалось сохранить пользователей: %1").arg(e.what()));
    }
}

void AdminInterface::clear_form() {
    name_edit->clear();
    login_edit->clear();
    password_edit->clear();
    role_combo->setCurrentIndex(0);
    group_id_edit->clear();
    course_edit->clear();
    subjects_edit->clear();
}

AdminInterface::~AdminInterface() {
    delete m_auth_system;
    delete m_schedule;
}

void AdminInterface::display() {
    show();
}
