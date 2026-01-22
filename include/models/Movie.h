#pragma once

#include <QObject>
#include <QString>

/**
 * @brief Represents a movie in the cinema system
 * 
 * This class encapsulates all information related to a movie,
 * including its unique identifier, title, duration, and genre.
 * It inherits from QObject to leverage Qt's meta-object system.
 */
class Movie : public QObject {
    Q_OBJECT
    Q_PROPERTY(int id READ getId CONSTANT)
    Q_PROPERTY(QString title READ getTitle CONSTANT)
    Q_PROPERTY(int duration READ getDuration CONSTANT)
    Q_PROPERTY(QString genre READ getGenre CONSTANT)

public:
    /**
     * @brief Constructs a new Movie object
     * @param id Unique movie identifier
     * @param title Movie title
     * @param duration Duration in minutes
     * @param genre Movie genre
     * @param parent Parent QObject for memory management
     */
    explicit Movie(int id, const QString& title, int duration, 
                   const QString& genre, QObject* parent = nullptr);
    
    /**
     * @brief Gets the movie ID
     * @return Unique movie identifier
     */
    int getId() const { return m_id; }
    
    /**
     * @brief Gets the movie title
     * @return Movie title
     */
    const QString& getTitle() const { return m_title; }
    
    /**
     * @brief Gets the movie duration
     * @return Duration in minutes
     */
    int getDuration() const { return m_duration; }
    
    /**
     * @brief Gets the movie genre
     * @return Movie genre
     */
    const QString& getGenre() const { return m_genre; }
    
    /**
     * @brief Equality operator based on ID
     * @param other Movie to compare with
     * @return true if movies have the same ID
     */
    bool operator==(const Movie& other) const { return m_id == other.m_id; }
    
private:
    int m_id;           ///< Unique movie identifier
    QString m_title;    ///< Movie title
    int m_duration;     ///< Duration in minutes
    QString m_genre;    ///< Movie genre
};
