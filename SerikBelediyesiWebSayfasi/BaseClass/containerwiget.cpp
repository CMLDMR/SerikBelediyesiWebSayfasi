#include "containerwiget.h"

#include <QFile>
#include <QFileInfo>
#include <QUuid>

ContainerWiget::ContainerWiget()
{

}

void ContainerWiget::setContainerStyle(ContainerWiget::ContainerStyleType type)
{
    switch (type) {
    case ROW:
        this->addStyleClass(Bootstrap::Grid::row);
        break;

    case CONTAINERFLUID:
        this->addStyleClass(Bootstrap::Grid::container_fluid);
        break;
    default:
        break;
    }
}

void ContainerWiget::showMessage(std::string title, std::string msg, std::string btnText)
{
    auto messageBox = this->addChild(
                Wt::cpp14::make_unique<Wt::WMessageBox>
                (title,
                 msg,
                 Wt::Icon::Information, StandardButton::Ok));
    if( btnText != "Tamam" )
    {
        auto btn = messageBox->button(StandardButton::Ok);
        btn->setText(btnText);
    }


        messageBox->buttonClicked().connect([=] {
            this->removeChild(messageBox);
        });

        messageBox->webWidget()->setZIndex(1000);
        messageBox->show();
}





FileUploaderWidget::FileUploaderWidget(mongocxx::database *_db)
    :DBClass (_db)
{

    mIsUploaded = false;

    auto fileUploaderContainer = this->addWidget(cpp14::make_unique<WContainerWidget>());
    fileUploaderContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(150,170),this->getRandom(170,190),this->getRandom(200,220)));
    fileUploaderContainer->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
    fileUploaderContainer->setMargin(10,Side::Top|Side::Bottom);

    mFileUploader = fileUploaderContainer->addWidget(cpp14::make_unique<WFileUpload>());
    auto progresBar = fileUploaderContainer->addWidget(cpp14::make_unique<WProgressBar>());
    mFileUploader->setFileTextSize(6000); // Set the maximum file size to 2048 kB.
    mFileUploader->setMargin(10, Wt::Side::Right);
    mFileUploader->setFilters("application/pdf");
    mFileUploader->setMultiple(false);
    mFileUploader->setProgressBar(progresBar);

    mFileUploader->fileTooLarge().connect([=] {
        std::cout << mFileUploader->fileTextSize() << std::endl;
        this->showMessage("Hata","Dosya Boyutu Fazla Büyük. Max: 6MB Olmalı");
    });

    Wt::WPushButton *uploadButton = fileUploaderContainer->addWidget(Wt::cpp14::make_unique<Wt::WPushButton>("PDF Cevap Yükle"));
    uploadButton->setMargin(10, Wt::Side::Left | Wt::Side::Right);

    uploadButton->clicked().connect([=] {

        mFileUploader->progressBar()->setValue(0);

        mFileUploader->upload();
        mFileUploader->enable();
        mFileUploader->setHidden(false);

    });

    mFileUploader->uploaded().connect([=] {
        auto list = mFileUploader->uploadedFiles();


        for( auto item : list )
        {
            QFileInfo info(item.clientFileName().c_str());

            QString _fileName = QString("%1%2.%3").arg(QUuid::createUuid().toString()).arg(item.clientFileName().c_str()).arg(info.suffix());

            mFileLocation = QString("docroot/tempfile/")+_fileName;
            mDoocRootLocation = QString("tempfile/")+ _fileName;
            if( QFile::copy(item.spoolFileName().c_str(),mFileLocation) )
            {
                _Uploaded.emit(NoClass());
                mIsUploaded = true;
            }else{
                mIsUploaded = false;
            }
        }

    });


}

bool FileUploaderWidget::isUploaded() const
{
    return mIsUploaded;
}

QString FileUploaderWidget::fileLocation() const
{
    return mFileLocation;
}

QString FileUploaderWidget::doocRootLocation() const
{
    return mDoocRootLocation;
}

Signal<NoClass> &FileUploaderWidget::Uploaded()
{
    return _Uploaded;
}
