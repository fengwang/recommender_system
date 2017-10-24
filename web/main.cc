#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WMenu>
#include <Wt/WPushButton>
#include <Wt/WStackedWidget>
#include <Wt/WText>
#include <Wt/WTextArea>
#include <Wt/WGroupBox>
#include <Wt/WHBoxLayout>
#include <Wt/WString>

#include <functional>

using namespace Wt;

/*
 * A simple hello world application class which demonstrates how to react
 * to events, read input, and give feed-back.
 */
struct MainApplication : public WApplication
{
    WContainerWidget *layout_container;
    WHBoxLayout *layout_hbox;

    MainApplication( const WEnvironment& env );

    /*
    WLineEdit* nameEdit_;
    WText* greeting_;
    */
};

/*
 * The env argument contains information about the new session, and
 * the initial request. It must be passed to the WApplication
 * constructor so it is typically also an argument for your custom
 * application constructor.
*/
MainApplication::MainApplication( const WEnvironment& env ) : 
    WApplication( env ), 
    layout_container( new WContainerWidget{} ),
    layout_hbox( new WHBoxLayout{} )
{
    setTitle( L"首页" );                             // application title

    (*this).useStyleSheet("./web/themes/bootstrap/3/bootstrap.css");

    layout_container->setLayout(layout_hbox);







    /*
    root()->addWidget( new WText( L"报上名来" ) ); // show some text
    nameEdit_ = new WLineEdit( root() );                   // allow text input
    nameEdit_->setFocus();                                 // give focus

    //WPushButton* button = new WPushButton( "Greet me.", root() );            // create a button
    WPushButton* button = new WPushButton( L"向我请安吧", root() );            // create a button
    button->setMargin( 5, Left );                          // add 5 pixels margin

    root()->addWidget( new WBreak() );                     // insert a line break

    greeting_ = new WText( root() );                       // empty text

    auto const& greetings = std::bind( [this]() { this->greeting_->setText("Hello there, " + this->nameEdit_->text()); } );

    nameEdit_->enterPressed().connect ( greetings );
    button->clicked().connect( greetings );
    */
}

WApplication* createApplication( const WEnvironment& env )
{
    /*
     * You could read information from the environment to decide whether
     * the user has permission to start a new application
     */
    return new MainApplication( env );
}

int main( int argc, char** argv )
{

    /*
     * Your main method may set up some shared resources, but should then
     * start the server application (FastCGI or httpd) that starts listening
     * for requests, and handles all of the application life cycles.
     *
     * The last argument to WRun specifies the function that will instantiate
     * new application objects. That function is executed when a new user surfs
     * to the Wt application, and after the library has negotiated browser
     * support. The function should return a newly instantiated application
     * object.
     */
    return WRun( argc, argv, &createApplication );
}

