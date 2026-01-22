#pragma once

#include <QObject>
#include <QString>

/**
 * @brief Represents a theater/cinema hall
 * 
 * This class encapsulates information about a cinema hall,
 * including its capacity and identification details.
 */
class Theater : public QObject {
    Q_OBJECT
    Q_PROPERTY(int id READ getId CONSTANT)
    Q_PROPERTY(QString name READ getName CONSTANT)
    Q_PROPERTY(int capacity READ getCapacity CONSTANT)

public:
    /// Default seating capacity for all theaters
    static constexpr int TOTAL_SEATS = 20;
    
    /**
     * @brief Constructs a new Theater object
     * @param id Unique theater identifier
     * @param name Theater name
     * @param capacity Seating capacity (default 20)
     * @param parent Parent QObject for memory management
     */
    explicit Theater(int id, const QString& name, int capacity = TOTAL_SEATS,
                    QObject* parent = nullptr);
    
    /**
     * @brief Gets the theater ID
     * @return Unique theater identifier
     */
    int getId() const { return m_id; }
    
    /**
     * @brief Gets the theater name
     * @return Theater name
     */
    const QString& getName() const { return m_name; }
    
    /**
     * @brief Gets the seating capacity
     * @return Number of seats in the theater
     */
    int getCapacity() const { return m_capacity; }
    
    /**
     * @brief Equality operator based on ID
     * @param other Theater to compare with
     * @return true if theaters have the same ID
     */
    bool operator==(const Theater& other) const { return m_id == other.m_id; }
    
private:
    int m_id;           ///< Unique theater identifier
    QString m_name;     ///< Theater name
    int m_capacity;     ///< Seating capacity
};
