/****************************************************************************
** Meta object code from reading C++ file 'admin_interface.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/admin_interface.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'admin_interface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSAdminInterfaceENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSAdminInterfaceENDCLASS = QtMocHelpers::stringData(
    "AdminInterface",
    "add_user",
    "",
    "delete_user",
    "add_lesson",
    "delete_lesson",
    "refresh_users_table",
    "refresh_schedule_table",
    "on_user_selected",
    "save_users_to_json"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSAdminInterfaceENDCLASS_t {
    uint offsetsAndSizes[20];
    char stringdata0[15];
    char stringdata1[9];
    char stringdata2[1];
    char stringdata3[12];
    char stringdata4[11];
    char stringdata5[14];
    char stringdata6[20];
    char stringdata7[23];
    char stringdata8[17];
    char stringdata9[19];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSAdminInterfaceENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSAdminInterfaceENDCLASS_t qt_meta_stringdata_CLASSAdminInterfaceENDCLASS = {
    {
        QT_MOC_LITERAL(0, 14),  // "AdminInterface"
        QT_MOC_LITERAL(15, 8),  // "add_user"
        QT_MOC_LITERAL(24, 0),  // ""
        QT_MOC_LITERAL(25, 11),  // "delete_user"
        QT_MOC_LITERAL(37, 10),  // "add_lesson"
        QT_MOC_LITERAL(48, 13),  // "delete_lesson"
        QT_MOC_LITERAL(62, 19),  // "refresh_users_table"
        QT_MOC_LITERAL(82, 22),  // "refresh_schedule_table"
        QT_MOC_LITERAL(105, 16),  // "on_user_selected"
        QT_MOC_LITERAL(122, 18)   // "save_users_to_json"
    },
    "AdminInterface",
    "add_user",
    "",
    "delete_user",
    "add_lesson",
    "delete_lesson",
    "refresh_users_table",
    "refresh_schedule_table",
    "on_user_selected",
    "save_users_to_json"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSAdminInterfaceENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   62,    2, 0x08,    1 /* Private */,
       3,    0,   63,    2, 0x08,    2 /* Private */,
       4,    0,   64,    2, 0x08,    3 /* Private */,
       5,    0,   65,    2, 0x08,    4 /* Private */,
       6,    0,   66,    2, 0x08,    5 /* Private */,
       7,    0,   67,    2, 0x08,    6 /* Private */,
       8,    0,   68,    2, 0x08,    7 /* Private */,
       9,    0,   69,    2, 0x08,    8 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject AdminInterface::staticMetaObject = { {
    QMetaObject::SuperData::link<Interface::staticMetaObject>(),
    qt_meta_stringdata_CLASSAdminInterfaceENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSAdminInterfaceENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSAdminInterfaceENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<AdminInterface, std::true_type>,
        // method 'add_user'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'delete_user'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'add_lesson'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'delete_lesson'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'refresh_users_table'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'refresh_schedule_table'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_user_selected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'save_users_to_json'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void AdminInterface::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<AdminInterface *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->add_user(); break;
        case 1: _t->delete_user(); break;
        case 2: _t->add_lesson(); break;
        case 3: _t->delete_lesson(); break;
        case 4: _t->refresh_users_table(); break;
        case 5: _t->refresh_schedule_table(); break;
        case 6: _t->on_user_selected(); break;
        case 7: _t->save_users_to_json(); break;
        default: ;
        }
    }
    (void)_a;
}

const QMetaObject *AdminInterface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AdminInterface::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSAdminInterfaceENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return Interface::qt_metacast(_clname);
}

int AdminInterface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Interface::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
