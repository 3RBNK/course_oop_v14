#ifndef COURSE_ADMIN_INTERFACE_H
#define COURSE_ADMIN_INTERFACE_H

#include <QWidget>
#include <QTabWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include "interface.h"
#include "user.h"
#include "auth_system.h"
#include "schedule.h"

class AdminInterface : public Interface {
Q_OBJECT

public:
    explicit AdminInterface(User *admin, QWidget *parent = nullptr);
    void display() override;

private slots:
    void add_user();
    void delete_user();

    void add_lesson();
    void delete_lesson();

    void refresh_users_table();
    void refresh_schedule_table();

    void on_user_selected();

    void save_users_to_json();
//    void save_schedule_to_json();
private:
    User *m_admin;
    AuthSystem *m_auth_system;
    Schedule *m_schedule;
    QTabWidget *tab_widget;

    QWidget *users_tab;
    QTableWidget *users_table;
    QLineEdit *name_edit;
    QLineEdit *login_edit;
    QLineEdit *password_edit;
    QComboBox *role_combo;
    QLineEdit *group_id_edit;
    QLineEdit *course_edit;
    QLineEdit *subjects_edit;

    QWidget *schedule_tab;
    QTableWidget *schedule_table;
    QComboBox *user_combo;
    QLineEdit *subject_edit;
    QComboBox *day_combo;
    QComboBox *time_combo;
    QLineEdit *auditorium_edit;
    QLineEdit *teacher_edit;

    void setup_ui();
    void setup_users_tab();
    void setup_schedule_tab();
    void populate_role_combo();
    void populate_user_combo();

    int generate_unique_user_id();
    int generate_unique_teacher_id();

    ~AdminInterface();
};

#endif //COURSE_ADMIN_INTERFACE_H