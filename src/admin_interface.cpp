#include "../include/admin_interface.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QDateTime>
#include <QInputDialog>
#include <QHeaderView>
#include <QDebug>
#include <QSharedPointer>

AdminInterface::AdminInterface(QWidget *parent)
        : Interface(parent) {
    m_auth_system = new AuthSystem();
    m_schedule = new Schedule();
    m_check_correct = new CheckCorrect();

    setup_ui();

    refresh_users_table();
    populate_user_combo();

    refresh_schedule_table();
}

void AdminInterface::setup_ui() {
    auto *main_layout = new QVBoxLayout(this);
    tab_widget = new QTabWidget(this);

    users_tab = new QWidget();
    schedule_tab_for_group = new QWidget();
    schedule_tab_for_teacher = new QWidget();

    setup_users_tab();
    setup_schedule_tab_for_group();
    setup_schedule_tab_for_teacher();

    tab_widget->addTab(users_tab, "Управление пользователями");
    tab_widget->addTab(schedule_tab_for_group, "Управление расписанием групп");
    tab_widget->addTab(schedule_tab_for_teacher, "Управление расписанием преподавателей");

    main_layout->addWidget(tab_widget);
    setLayout(main_layout);
    setWindowTitle("Панель администратора");
    resize(800, 600);
}

void AdminInterface::setup_users_tab() {
    auto *layout = new QVBoxLayout(users_tab);

    users_table = new QTableWidget(0, 7, users_tab);
    users_table->setHorizontalHeaderLabels({"ID", "Имя", "Роль", "Логин", "Пароль", "Курс", "Предметы"});
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

void AdminInterface::setup_schedule_tab_for_group() {
    auto *layout = new QVBoxLayout(schedule_tab_for_group);

    schedule_table_group = new QTableWidget(6, 7);
    QStringList days = {"Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота", "Воскресение"};
    QStringList times = {"08:15-09:50", "10:00-11:35", "11:45-13:20",
                         "14:20-15:55", "16:05-17:40", "17:50-19:25"};

    schedule_table_group->setHorizontalHeaderLabels(days);
    schedule_table_group->setVerticalHeaderLabels(times);

    user_combo_group = new QComboBox();

    subject_edit_group = new QLineEdit();
    subject_edit_group->setPlaceholderText("Предмет");

    day_combo_group = new QComboBox();
    day_combo_group->addItems(days);

    time_combo_group = new QComboBox();
    time_combo_group->addItems(times);

    auditorium_edit_group = new QLineEdit();
    auditorium_edit_group->setPlaceholderText("Аудитория");

    teacher_edit_group = new QLineEdit();
    teacher_edit_group->setPlaceholderText("Преподаватель");

    auto *form_layout = new QHBoxLayout();
    auto *add_lesson_btn = new QPushButton("Добавить занятие");
    auto *delete_lesson_btn = new QPushButton("Удалить занятие");
    form_layout->addWidget(user_combo_group);
    form_layout->addWidget(subject_edit_group);
    form_layout->addWidget(day_combo_group);
    form_layout->addWidget(time_combo_group);
    form_layout->addWidget(auditorium_edit_group);
    form_layout->addWidget(teacher_edit_group);
    form_layout->addWidget(add_lesson_btn);
    form_layout->addWidget(delete_lesson_btn);

    layout->addWidget(schedule_table_group);
    layout->addLayout(form_layout);

    connect(add_lesson_btn, &QPushButton::clicked, this, &AdminInterface::add_lesson);
    connect(delete_lesson_btn, &QPushButton::clicked, this, &AdminInterface::delete_lesson);
    connect(user_combo_group, &QComboBox::currentTextChanged, this, &AdminInterface::refresh_schedule_table);
}

void AdminInterface::setup_schedule_tab_for_teacher() {
    auto *layout = new QVBoxLayout(schedule_tab_for_teacher);

    schedule_table_teacher = new QTableWidget(6, 7);
    QStringList days = {"Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота", "Воскресение"};
    QStringList times = {"08:15-09:50", "10:00-11:35", "11:45-13:20",
                         "14:20-15:55", "16:05-17:40", "17:50-19:25"};
    schedule_table_teacher->setHorizontalHeaderLabels(days);
    schedule_table_teacher->setVerticalHeaderLabels(times);

    user_combo_teacher = new QComboBox();

    subject_edit_teacher = new QLineEdit();
    subject_edit_teacher->setPlaceholderText("Предмет");

    day_combo_teacher = new QComboBox();
    day_combo_teacher->addItems(days);

    time_combo_teacher = new QComboBox();
    time_combo_teacher->addItems(times);

    auditorium_edit_teacher = new QLineEdit();
    auditorium_edit_teacher->setPlaceholderText("Аудитория");

    group_edit_teacher = new QLineEdit();
    group_edit_teacher->setPlaceholderText("Группа");

    auto *form_layout = new QHBoxLayout();
    auto *add_lesson_btn = new QPushButton("Добавить занятие");
    auto *delete_lesson_btn = new QPushButton("Удалить занятие");
    form_layout->addWidget(user_combo_teacher);
    form_layout->addWidget(subject_edit_teacher);
    form_layout->addWidget(day_combo_teacher);
    form_layout->addWidget(time_combo_teacher);
    form_layout->addWidget(auditorium_edit_teacher);
    form_layout->addWidget(group_edit_teacher);
    form_layout->addWidget(add_lesson_btn);
    form_layout->addWidget(delete_lesson_btn);

    layout->addWidget(schedule_table_teacher);
    layout->addLayout(form_layout);

    connect(add_lesson_btn, &QPushButton::clicked, this, &AdminInterface::add_lesson);
    connect(delete_lesson_btn, &QPushButton::clicked, this, &AdminInterface::delete_lesson);
    connect(user_combo_teacher, &QComboBox::currentTextChanged, this, &AdminInterface::refresh_schedule_table);
}

void AdminInterface::populate_user_combo() {
    user_combo_group->clear();
    user_combo_teacher->clear();

    for (const auto *user: m_auth_system->m_users) {
        if (user->role() == "group") {
            user_combo_group->addItem(user->name());
        } else if (user->role() == "teacher") {
            user_combo_teacher->addItem(user->name());
        }
    }
}

void AdminInterface::refresh_schedule_table() {
    int current_tab_index = tab_widget->currentIndex();
    if (current_tab_index == tab_widget->indexOf(schedule_tab_for_group)) {
        for (int i = 0; i < schedule_table_group->rowCount(); ++i) {
            for (int j = 0; j < schedule_table_group->columnCount(); ++j) {
                if (schedule_table_group->item(i, j)) {
                    delete schedule_table_group->item(i, j);
                }
            }
        }
    } else {
        for (int i = 0; i < schedule_table_teacher->rowCount(); ++i) {
            for (int j = 0; j < schedule_table_teacher->columnCount(); ++j) {
                if (schedule_table_teacher->item(i, j)) {
                    delete schedule_table_teacher->item(i, j);
                }
            }
        }
    }

    QString selected_user_name;
    if (current_tab_index == tab_widget->indexOf(schedule_tab_for_group)) {
        selected_user_name = user_combo_group->currentText();
    } else if (current_tab_index == tab_widget->indexOf(schedule_tab_for_teacher)) {
        selected_user_name = user_combo_teacher->currentText();
    } else {
        return;
    }

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
        int day = time_slot.date().dayOfWeek();
        int column = day % 7;

        QTime lesson_time = time_slot.time();

        int row = -1;
        if (lesson_time >= QTime(8, 15) && lesson_time <= QTime(9, 50)) row = 0;
        else if (lesson_time >= QTime(10, 0) && lesson_time <= QTime(11, 35)) row = 1;
        else if (lesson_time >= QTime(11, 45) && lesson_time <= QTime(13, 20)) row = 2;
        else if (lesson_time >= QTime(14, 20) && lesson_time <= QTime(15, 55)) row = 3;
        else if (lesson_time >= QTime(16, 5) && lesson_time <= QTime(17, 40)) row = 4;
        else if (lesson_time >= QTime(17, 50) && lesson_time <= QTime(19, 25)) row = 5;

        if (row == -1) {
            continue;
        }

        QString text = lesson->subject() + "\n" + lesson->auditorium().auditorium_info();
        auto *item = new QTableWidgetItem(text);
        item->setTextAlignment(Qt::AlignCenter);
        if (current_tab_index == tab_widget->indexOf(schedule_tab_for_group)) {
            schedule_table_group->setItem(row, column, item);
        } else {
            schedule_table_teacher->setItem(row, column, item);
        }
    }
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

    if (role == "student") {
        bool ok;

        int course = QInputDialog::getInt(this, "Курс студента",
                                          "Введите номер курса:", 1, 1, 6, 1, &ok);
        if (!ok) {
            QMessageBox::warning(this, "Ошибка", "Необходимо указать курс");
            return;
        }

        int group_id = QInputDialog::getInt(this, "Группа студента",
                                            "Введите ID группы:", 1, 1, 999999, 1, &ok);
        if (!ok) {
            QMessageBox::warning(this, "Ошибка", "Необходимо указать ID группы");
            return;
        }

        QString group_name = QInputDialog::getText(this, "Имя группы", "Введите название группы", QLineEdit::Normal, "",
                                                   &ok);
        if (!ok) {
            QMessageBox::warning(this, "Ошибка", "Необходимо указать имя группы");
            return;
        }

        int user_id = generate_unique_user_id();
        QList<User *> students = {new User(user_id, name, role, login, password)};
        auto *group = new Group(group_id, group_name, "group", login, password, course, students);
        m_auth_system->add_user(group);
    } else if (role == "teacher") {
        bool ok;
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

        int teacher_id = generate_unique_teacher_id();
        auto *teacher = new Teacher(teacher_id, name, role, login, password, subjects);
        m_auth_system->add_user(teacher);
    } else {
        int admin_id = generate_unique_user_id();
        auto *admin = new User(admin_id, name, role, login, password);
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
    QString user_role = users_table->item(row, 2)->text();

    if (user_role == "student") {
        for (int i = 0; i < m_auth_system->m_users.size(); ++i) {
            if (auto *group = dynamic_cast<Group *>(m_auth_system->m_users[i])) {
                for (auto *student: group->students()) {
                    if (student->id() == user_id) {
                        group->delete_student(student);
                        delete student;
                        break;
                    }
                }
            }
        }
    } else {
        for (int i = 0; i < m_auth_system->m_users.size(); ++i) {
            if (m_auth_system->m_users[i]->id() == user_id) {
                delete m_auth_system->m_users[i];
                m_auth_system->m_users.removeAt(i);
                break;
            }
        }
    }

    refresh_users_table();
    populate_user_combo();
}


void AdminInterface::add_lesson() {
    QComboBox * active_user_combo = nullptr;
    if (tab_widget->currentIndex() == tab_widget->indexOf(schedule_tab_for_group)) {
        active_user_combo = user_combo_group;
    } else if (tab_widget->currentIndex() == tab_widget->indexOf(schedule_tab_for_teacher)) {
        active_user_combo = user_combo_teacher;
    } else {
        QMessageBox::warning(this, "Ошибка", "Неизвестная вкладка!");
        return;
    }

    QString selected_user_name = active_user_combo->currentText();

    if (selected_user_name.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Выберите пользователя!");
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
        QMessageBox::warning(this, "Ошибка", "Пользователь не найден!");
        return;
    }

    QString subject;
    QString auditorium_name;
    QString teacher_or_group_name;

    if (tab_widget->currentIndex() == tab_widget->indexOf(schedule_tab_for_group)) {
        subject = subject_edit_group->text();
        auditorium_name = auditorium_edit_group->text();
        teacher_or_group_name = teacher_edit_group->text();
    } else {
        subject = subject_edit_teacher->text();
        auditorium_name = auditorium_edit_teacher->text();
        teacher_or_group_name = group_edit_teacher->text();
    }

    if (subject.isEmpty() || auditorium_name.isEmpty() || teacher_or_group_name.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Все поля должны быть заполнены!");
        return;
    }

    int day_index;
    if (tab_widget->currentIndex() == tab_widget->indexOf(schedule_tab_for_group)) {
        day_index = day_combo_group->currentIndex();
    } else {
        day_index = day_combo_teacher->currentIndex();
    }

    QString time_slot_str;
    if (tab_widget->currentIndex() == tab_widget->indexOf(schedule_tab_for_group)) {
        time_slot_str = time_combo_group->currentText();
    } else {
        time_slot_str = time_combo_teacher->currentText();
    }
    QString start_time_str = time_slot_str.split('-').first();
    QTime lesson_time = QTime::fromString(start_time_str, "HH:mm");

    if (!m_check_correct->validate_schedule(m_schedule->lessons(), day_index, lesson_time)) {
        QMessageBox::critical(this, "Ошибка", "В это время есть занятие");
    }

    QDate current_date = QDate::currentDate();
    int days_offset = day_index - (current_date.dayOfWeek() % 7);
    QDate lesson_date = current_date.addDays(days_offset);

    QDateTime lesson_datetime(lesson_date, lesson_time);
    Auditorium auditorium(auditorium_name);


    int lesson_id = generate_unique_lesson_id();
    QSharedPointer <Lesson> new_lesson;
    if (tab_widget->currentIndex() == tab_widget->indexOf(schedule_tab_for_group)) {
        Teacher teacher(teacher_or_group_name);
        QList < Group > groups = {Group(0, selected_user->name(), "group", "", "", 0, {})};
        new_lesson = QSharedPointer<Lesson>::create(lesson_id, subject, teacher, groups, lesson_datetime, auditorium);
    } else {
        Teacher teacher(teacher_or_group_name);

        QList < Group > groups;
        QStringList group_names = teacher_or_group_name.split(",", Qt::SkipEmptyParts);
        for (auto group_name: group_names) {
            groups.append(
                    Group(0, group_name, "group", "", "", 0, {})
            );
        }

        new_lesson = QSharedPointer<Lesson>::create(lesson_id, subject, teacher, groups, lesson_datetime, auditorium);
    }


    m_schedule->load_from_json("D:\\home_work\\oop\\course\\code_v1\\users\\users.json", *selected_user);
    m_schedule->add_lesson(new_lesson);
    m_schedule->save_to_json("D:\\home_work\\oop\\course\\code_v1\\users\\users.json", *selected_user);

    refresh_schedule_table();

    if (tab_widget->currentIndex() == tab_widget->indexOf(schedule_tab_for_group)) {
        subject_edit_group->clear();
        auditorium_edit_group->clear();
        teacher_edit_group->clear();
    } else {
        subject_edit_teacher->clear();
        auditorium_edit_teacher->clear();
        group_edit_teacher->clear();
    }

    QMessageBox::information(this, "Успех", "Занятие успешно добавлено!");
}

void AdminInterface::delete_lesson() {
    QComboBox * active_user_combo = nullptr;
    if (tab_widget->currentIndex() == tab_widget->indexOf(schedule_tab_for_group)) {
        active_user_combo = user_combo_group;
    } else if (tab_widget->currentIndex() == tab_widget->indexOf(schedule_tab_for_teacher)) {
        active_user_combo = user_combo_teacher;
    } else {
        QMessageBox::warning(this, "Ошибка", "Неизвестная вкладка!");
        return;
    }

    QString selected_user_name = active_user_combo->currentText();
    if (selected_user_name.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Выберите пользователя!");
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
        QMessageBox::warning(this, "Ошибка", "Пользователь не найден!");
        return;
    }

    QList<QTableWidgetItem *> selected_items;
    if (tab_widget->currentIndex() == tab_widget->indexOf(schedule_tab_for_group)) {
        selected_items = schedule_table_group->selectedItems();
    } else {
        selected_items = schedule_table_teacher->selectedItems();
    }

    if (selected_items.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Выберите занятие для удаления!");
        return;
    }

    int row = selected_items.first()->row();
    int column = selected_items.first()->column();

    QStringList times = {"08:15", "10:00", "11:45", "14:20", "16:05", "17:50"};
    if (row < 0 || row >= times.size()) {
        QMessageBox::warning(this, "Ошибка", "Некорректная строка таблицы!");
        return;
    }

    QTime lesson_time = QTime::fromString(times[row], "HH:mm");
    int day_of_week = column % 7;

    m_schedule->load_from_json("D:\\home_work\\oop\\course\\code_v1\\users\\users.json", *selected_user);
    QList < QSharedPointer < Lesson >> lessons = m_schedule->lessons();

    for (const auto &lesson: lessons) {
        if (lesson->time_slot().time() == lesson_time &&
            lesson->time_slot().date().dayOfWeek() % 7 == day_of_week) {
            m_schedule->remove_lesson(lesson->lesson_id());
            break;
        }
    }

    m_schedule->save_to_json("D:\\home_work\\oop\\course\\code_v1\\users\\users.json", *selected_user);
    refresh_schedule_table();
}

void AdminInterface::refresh_users_table() {
    users_table->setRowCount(0);
    int row = 0;

    for (const auto *user: m_auth_system->m_users) {
        if (const auto *group = dynamic_cast<const Group *>(user)) {
            for (const auto *student: group->students()) {
                users_table->insertRow(row);
                users_table->setItem(row, 0, new QTableWidgetItem(QString::number(student->id())));
                users_table->setItem(row, 1, new QTableWidgetItem(student->name()));
                users_table->setItem(row, 2, new QTableWidgetItem(student->role()));
                users_table->setItem(row, 3, new QTableWidgetItem(student->login()));
                users_table->setItem(row, 4, new QTableWidgetItem(student->password()));
                users_table->setItem(row, 5, new QTableWidgetItem(QString::number(group->course())));
                row++;
            }
        } else {
            users_table->insertRow(row);
            users_table->setItem(row, 0, new QTableWidgetItem(QString::number(user->id())));
            users_table->setItem(row, 1, new QTableWidgetItem(user->name()));
            users_table->setItem(row, 2, new QTableWidgetItem(user->role()));
            users_table->setItem(row, 3, new QTableWidgetItem(user->login()));
            users_table->setItem(row, 4, new QTableWidgetItem(user->password()));

            if (const auto *teacher = dynamic_cast<const Teacher *>(user)) {
                users_table->setItem(row, 6, new QTableWidgetItem(teacher->subjects().join(", ")));
            }

            row++;
        }
    }
}


int AdminInterface::generate_unique_user_id() {
    int max_id = 0;
    for (const auto *user: m_auth_system->get_users()) {
        if (user->id() > max_id) {
            max_id = user->id();
        }
    }
    return max_id + 1;
}

int AdminInterface::generate_unique_lesson_id() {
    int max_id = 500;
    for (const auto &lesson: m_schedule->lessons()) {
        if (lesson->lesson_id() > max_id) {
            max_id = lesson->lesson_id();
        }
    }

    return max_id + 1;
}

int AdminInterface::generate_unique_teacher_id() {
    int max_id = 1000;
    for (const auto *user: m_auth_system->get_users()) {
        if (const auto *teacher = dynamic_cast<const Teacher *>(user)) {
            if (teacher->id() > max_id) {
                max_id = teacher->id();
            }
        }
    }
    return max_id + 1;
}

void AdminInterface::populate_role_combo() {
    role_combo->clear();
    role_combo->addItems({"student", "teacher", "admin"});
}


void AdminInterface::save_users_to_json() {
    if (m_check_correct->validate_users(m_auth_system->m_users)) {
        m_auth_system->save_users_to_json("D:\\home_work\\oop\\course\\code_v1\\users\\users.json");
        QMessageBox::information(this, "Успех", "Пользователи успешно сохранены в файл");
    } else {
        QMessageBox::critical(this, "Ошибка", QString("Ошибка: %1").arg(m_check_correct->errors().join("\n")));
    }
}


AdminInterface::~AdminInterface() {
    delete m_auth_system;
    delete m_schedule;
}

void AdminInterface::display() {
    show();
}