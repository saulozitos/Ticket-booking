#include "cli/CLIInterface.h"
#include <QCoreApplication>
#include <QTextStream>

/**
 * @brief Main entry point for the CLI application
 * @param argc Argument count
 * @param argv Argument values
 * @return Exit code
 */
int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("Ticket Booking System");
    QCoreApplication::setApplicationVersion("1.0.0");
    
    try {
        CLIInterface cli;
        cli.run();
        return 0;
    } catch (const std::exception& e) {
        QTextStream(stderr) << "Error: " << e.what() << "\n";
        return 1;
    }
}
