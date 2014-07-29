#include "ShortcutEdit.h"
#include <QKeyEvent>
#include <QDebug>


ShortcutEdit::ShortcutEdit(QWidget *parent) :
    QLineEdit(parent)
{
}


const QKeySequence ShortcutEdit::getKeysequence() const
{
    // returns current keystroke combination
    return QKeySequence(keyInt);
}

void ShortcutEdit::setErrorState(bool error){
    if(error){
        setStyleSheet("color: #FF0000");
    }else{
        setStyleSheet("color: #000000");
    }
}

void ShortcutEdit::keyPressEvent(QKeyEvent *event)
{

    keyInt = event->key();
    // find key-id
    const Qt::Key key = static_cast<Qt::Key>(keyInt);

    // we do not know how to handle this case
    if( key == Qt::Key_unknown )
        return;

    // these keys will be ignored
    if( key == Qt::Key_Escape || key == Qt::Key_Backspace )
    {
        setText("");
        return;
    }

    // any combination of "Ctrl, Alt, Shift" ?
    Qt::KeyboardModifiers modifiers = event->modifiers();
    if(modifiers.testFlag(Qt::ShiftModifier))
        keyInt += Qt::SHIFT;
    if(modifiers.testFlag(Qt::ControlModifier))
        keyInt += Qt::CTRL;
    if(modifiers.testFlag(Qt::AltModifier))
        keyInt += Qt::ALT;

    // some strange cases (only Ctrl)
    QString KeyText = QKeySequence(keyInt).toString(QKeySequence::NativeText) ;
    for(int i=0;i<KeyText.length();i++){
        if(KeyText[i].toAscii()==0){
            setText("");
            return;
        }
    }


    // display key combination
    setText( QKeySequence(keyInt).toString(QKeySequence::NativeText) );
    // do not forward keypress-event
    event->setAccepted(true);

    // everything is fine , so ask for saving
    emit askForSave();
}
