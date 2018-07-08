

#ifndef TOOL_CONVERT_H
#define TOOL_CONVERT_H

#include <QByteArray>
#include <QVBoxLayout>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QTabWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QComboBox>
#include <QDebug>

//数据缓冲区和数据类型描述，设计上允许拷贝（效率问题不大）
class CodeData {
public:
    enum DataType {
        TYPE_NONE = 0,
        TYPE_BYTES = 1,          //字节流

        TYPE_TEXT_MIN,           //字符集文本
        TYPE_TEXT_UTF8,
        TYPE_TEXT_UTF16,
        TYPE_TEXT_UTF32,
        TYPE_TEXT_GBK,
        TYPE_TEXT_MAX,

        TYPE_MAX,
    };

    CodeData() {
        return;
    }
    CodeData(const CodeData& from) {
        this->m_buf.clear();
        this->m_buf.append(from.m_buf);
        return;
    }
    ~CodeData() {
        return;
    }

    QByteArray m_buf;
    DataType m_type = TYPE_NONE;
};

class DataView : public QWidget
{
    Q_OBJECT
public:
    DataView(QWidget *parent = 0);
    ~DataView() {}
    void setData(CodeData *data);   //If data is null, clear data view.
private:
    CodeData m_data;                //这里保存拷贝了

    QTabWidget *tabwidget = NULL;
    //Hex
    QWidget *hex_widget = NULL;
    QTextEdit *hex_edit = NULL;
    //ASCII
    QWidget *ascii_widget = NULL;
    QTextEdit *ascii_edit = NULL;
    //GBK
    QWidget *gbk_widget = NULL;
    QTextEdit *gbk_edit = NULL;
    //UTF8
    QWidget *utf8_widget = NULL;
    QTextEdit *utf8_edit = NULL;
    //UTF16
    QWidget *utf16_widget = NULL;
    QTextEdit *utf16_edit = NULL;
    //UTF32
    QWidget *utf32_widget = NULL;
    QTextEdit *utf32_edit = NULL;
    //Others
    QWidget *others_widget = NULL;
    QComboBox *others_code = NULL;
    QTextEdit *others_edit = NULL;
};

//每个节点都是一个Coder的子类
class Coder :public QWidget
{
    Q_OBJECT
public:
    Coder(QWidget *parent = 0);
    virtual ~Coder() {}

    QString name() {return m_name;}
    QString desc() {return m_desc;}
    void setName(QString name) {this->m_name = name;}
    void setDesc(QString desc) {this->m_desc = desc;}

    //TODO 刷新信号等，需要链式功能

    virtual CodeData flushChain(CodeData input) = 0;        //输入输出
private:
    QString m_name = "";
    QString m_desc = "";
};

//Coder子类，CoderInput
class CoderInput : public Coder
{
    Q_OBJECT
public:
    CoderInput(QWidget *parent = 0);
    ~CoderInput() {}

    CodeData flushChain(CodeData input);
    static Coder* make() {return new CoderInput();}
private:
    CodeData m_data;

    QTextEdit *text_edit = NULL;

    QLineEdit *file_path = NULL;
    QLabel *file_err = NULL;

    DataView *data_view = NULL;

    void flushText();
    void flushFile();
};

//Coder子类，CoderUrlEncode
class CoderUrlEncode : public Coder
{
    Q_OBJECT
public:
    CoderUrlEncode(QWidget *parent = 0);
    ~CoderUrlEncode() {}

    CodeData flushChain(CodeData input);
    static Coder* make() {qDebug() << "CoderUrlEncode::make";return new CoderUrlEncode();}
private:
};

//Coder子类，CoderUrlDecode
class CoderUrlDecode : public Coder
{
    Q_OBJECT
public:
    CoderUrlDecode(QWidget *parent = 0);
    ~CoderUrlDecode() {}

    CodeData flushChain(CodeData input);
    static Coder* make() {qDebug() << "CoderUrlDecode::make";return new CoderUrlDecode();}
private:
};


//为Coder及其子类提供下边缘调整大小高矮功能
class CoderBox :public QWidget
{
    Q_OBJECT
public:
    CoderBox(QWidget *parent = 0);
    ~CoderBox() {}

    void setCoder(Coder *coder);
signals:
    void signalsAdd();
    void signalsDel();
private:
    QLabel *title_label = NULL;
    QPushButton *delcur = NULL;
    QHBoxLayout *coder_layout = NULL;
    Coder *coder = NULL;
    QPushButton *addnew = NULL;
};

//选择Coder的Dialog
class WhichCoder :public QDialog
{
    Q_OBJECT
public:
    WhichCoder(QDialog *parent = 0);
    ~WhichCoder() {}

    static Coder *getCoder();
private:
    Coder *m_coder = NULL;
};

//ConvertInner为主要实现(用QWidget不好使，需要继承实现)
class ConvertInner : public QWidget
{
    Q_OBJECT
public:
    ConvertInner(QWidget *parent = 0);
    ~ConvertInner() {}
};

//ConvertDialog为ConvertInner提供滚动条功能
class ConvertDialog : public QWidget
{
    Q_OBJECT
public:
    static void launch();
public:
    ConvertDialog(QWidget *parent = 0);
    ~ConvertDialog() {}
public:
};

#endif //CONVERT
