#ifndef ATCMOUSEHANDLER_H
#define ATCMOUSEHANDLER_H

#define ATC_MOUSE_HANDLER                               \
    void on_buttonClose_clicked()                       \
    {                                                   \
        close();                                        \
    }                                                   \
                                                        \
    void on_buttonMinMax_clicked()                      \
    {                                                   \
        if(isMaximized())                               \
        {                                               \
            minimizeWindow();                           \
        }                                               \
        else                                            \
        {                                               \
            maximizeWindow();                           \
        }                                               \
    }                                                   \
                                                        \
    void on_buttonClose_pressed()                       \
    {                                                   \
        setFlagStdButtonPressed(true);                  \
    }                                                   \
                                                        \
    void on_buttonMinMax_pressed()                      \
    {                                                   \
        setFlagStdButtonPressed(true);                  \
    }                                                   \
                                                        \
    void on_buttonClose_released()                      \
    {                                                   \
        setFlagStdButtonPressed(false);                 \
    }                                                   \
                                                        \
    void on_buttonMinMax_released()                     \
    {                                                   \
        setFlagStdButtonPressed(false);                 \
    }

#endif // ATCMOUSEHANDLER_H
