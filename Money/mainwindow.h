#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void loadBankData();
    void loadAccountData();
    void loadCCAccountData();

private slots:

    void on_pBExit_clicked();

    void on_pBNBankAsset_clicked();

    void on_pushButtonSaveBanks_clicked();

    void on_pBNExpAcct_clicked();

    void on_pushButtonSaveAccts_clicked();

    void on_pBNCCardAcct_clicked();

    void on_pushButtonSaveCCAcct_clicked();

    void on_pBNExpTrans_clicked();

    void on_pushButtonSaveExpTx_clicked();

    void on_pBNCreditTrans_clicked();

    void on_pushButtonSaveCCTx_clicked();

    void on_pBPCAcct_clicked();

    void on_pBLAssets_clicked();

    void on_pushButtonDispAssets_clicked();

    void on_pushButtonSavePayCCTx_2_clicked();

    void on_pBLCCAccts_clicked();

    void on_pushButtonCCBalances_clicked();

    void on_pushButtonTxByDate_clicked();

    void on_pBVTransByDate_clicked();

    void on_pBPMTransNC_clicked();

    void on_pushButtonTextSearch_clicked();

    void on_pushButtonMemExpTx_clicked();

    void on_pushButtonFindMemExpTx_clicked();

    void on_pushButtonFindMemTx_2_clicked();

    void on_pushButtonFindMemTx_clicked();

    void on_pushButtonMemCCTx_2_clicked();

    void on_pushButtonMemCCPayTx_clicked();

    void on_pushButtonViewMemTx_clicked();

    void on_pushButtonViewMemTx_2_clicked();

    void on_pushButtonViewBillsDue_clicked();

    void on_calendarWidget_clicked(const QDate &date);

    void on_pushButtonDeleteExp_clicked();

    void on_pushButtonDeleteCCTx_clicked();

    void on_pushButtonDeletePayTx_clicked();

    void on_pushButtonResetAssets_clicked();

    void on_pushButtonResetCC_clicked();

    void on_pushButtonSearchAcct_clicked();

    void on_pushButtonDeleteSelectTx_clicked();

    void on_pushButtonSearchAmt_clicked();

    void on_pushButtonDelectMemTx_clicked();

    void on_pushButtonViewExp_2_clicked();

    void on_pushButtonViewExp_clicked();

    void on_pushButtonDeleteBank_clicked();

    void on_pushButtonDeleteAccount_clicked();

    void on_pushButtonDeleteCreditAccount_clicked();

    void on_pushButtonDeleteMemTx_clicked();

    void on_pushButtonModifyBudget_clicked();

    void on_pushButtonViewTotals_clicked();

    void on_pushButtonSetBudgets_clicked();

    void on_checkBox_toggled(bool checked);

    void on_pushButtonNewFiscalYear_clicked();



private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
