#ifndef SAMPLE_ENTRIES_H
#define SAMPLE_ENTRIES_H

#include <vector>
#include <QDate>
#include "../model/journalentry.h"

class SampleEntries
{
public:
    static std::vector<JournalEntry> getSampleEntries()
    {
        std::vector<JournalEntry> entries;

        // Entry 1 - Recent coding session
        entries.emplace_back(
            "entry-001",
            QDate(2025, 6, 3),
            "Completed Qt Journal Application",
            "Today I finished implementing the Qt journal application. Added filtering functionality "
            "and the undo/redo system. The interface looks quite good with QTreeWidget for displaying entries. "
            "Next step is to add more robust validations and improve the UI design."
        );

        // Entry 2 - Weekend reflection
        entries.emplace_back(
            "entry-002",
            QDate(2025, 6, 1),
            "Relaxing mountain weekend",
            "Spent a wonderful weekend in the Carpathian Mountains. The weather was perfect for hiking. "
            "Climbed Omu Peak and the view was spectacular. Realized how important it is to break away "
            "from daily routine and reconnect with nature. Planning to do this more often."
        );

        // Entry 3 - Work project
        entries.emplace_back(
            "entry-003",
            QDate(2025, 5, 28),
            "Project presentation at work",
            "Today I presented the automation project to the team. The feedback was positive and "
            "management seems interested in implementing the solution. Received some valuable suggestions "
            "for improvements. Happy that weeks of work are paying off."
        );

        // Entry 4 - Personal development
        entries.emplace_back(
            "entry-004",
            QDate(2025, 5, 25),
            "Started machine learning course",
            "Began the online machine learning course I've been eyeing for a while. "
            "First lesson was about linear regression algorithms. Seems quite complex, "
            "but I'm determined to complete it. The goal is to apply this knowledge in future projects."
        );

        // Entry 5 - Family time
        entries.emplace_back(
            "entry-005",
            QDate(2025, 5, 22),
            "Mom's birthday celebration",
            "Mom's birthday celebration was special. The whole family gathered and we cooked "
            "her favorite meal together. Seeing how happy she was to have us all together. "
            "These moments remind me of the importance of family and time spent together."
        );

        // Entry 6 - Learning experience
        entries.emplace_back(
            "entry-006",
            QDate(2025, 5, 20),
            "Design patterns workshop",
            "Attended a workshop about design patterns in programming. Learned about "
            "Singleton, Observer, and Factory patterns. The instructor was excellent and gave practical "
            "examples from his experience. Planning to implement these concepts in my current projects."
        );

        // Entry 7 - Health and fitness
        entries.emplace_back(
            "entry-007",
            QDate(2025, 5, 18),
            "First 10km run achievement",
            "Today I managed to run 10km for the first time! Started training 2 months ago "
            "and finally reached this goal. Feel very proud of the progress made. "
            "Next objective is to participate in a 15km race in September."
        );

        // Entry 8 - Cultural experience
        entries.emplace_back(
            "entry-008",
            QDate(2025, 5, 15),
            "Symphony concert at Romanian Athenaeum",
            "Attended an extraordinary symphony concert at the Romanian Athenaeum. The orchestra performed "
            "Beethoven's 9th Symphony. The hall's acoustics are impressive and the performance was "
            "world-class. Culture and art have the power to inspire us and make us feel more human."
        );

        // Entry 9 - Travel planning
        entries.emplace_back(
            "entry-009",
            QDate(2025, 5, 12),
            "Summer vacation planning",
            "Started planning my summer vacation. Thinking about visiting Greece for the first time. "
            "Read about the Greek islands and they seem absolutely charming. Santorini and Mykonos are "
            "at the top of my list. Need to make reservations as soon as possible."
        );

        // Entry 10 - Reflection on goals
        entries.emplace_back(
            "entry-010",
            QDate(2025, 5, 10),
            "Mid-year goals reflection",
            "At mid-year, it's time to evaluate the goals set in January. "
            "Made good progress with learning new technologies and improving physical fitness. "
            "However, I need to focus more on reading and developing creative hobbies. "
            "Plan for the coming months includes more reading and maybe start learning to play guitar."
        );

        return entries;
    }


    static void populateRepository(JournalController* controller)
    {
        auto entries = getSampleEntries();
        for (const auto& entry : entries) {
            try {
                controller->addEntry(entry.id(), entry.date(), entry.title(), entry.content());
            } catch (const std::exception& e) {
                // Entry might already exist, skip silently
                continue;
            }
        }
    }
};

#endif // SAMPLE_ENTRIES_H