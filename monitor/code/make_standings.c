#include <stdio.h>
#include <time.h>
#include <string.h>
#include "player_database.h"
#include "resources.h"

void makeStandings(int rankings[], double wealth[], int total_players, int contest_end)
{
    char time_stamp[TIME_STAMP_SIZE];
    memset(time_stamp, 0, sizeof(time_stamp));
    time_t timeStamp;
    time(&timeStamp);
    struct tm myTime;
    localtime_r(&timeStamp, &myTime);
    strftime(time_stamp, TIME_STAMP_SIZE, SCORECARD_TIME_STAMP_FORMAT, &myTime);

    FILE *score;
    score = fopen (getScorecardPath(), "w");
    if(score)
    {
        fprintf(score, 
                "<html>\n"
                "<head>\n"
                "<META http-equiv=\"Refresh\" CONTENT=\"59;url=%s%s\">\n"
                "<title>%s</title>\n"
                "</head>\n"
                "<body bgcolor=\"#ffffdf\">\n"
                "<center>\n"
                "<table cellspacing=10>\n"
                "<tr>\n"
                "<th><b><font size=+2 color=\"#BB0000\">%s</font></b></th>\n"
                "</tr>\n"
                "</table><p>\n"
                "<font size=+2 color=\"#0000BB\"><b><nobr>Scoreboard</nobr></b></font><p>\n"
                "<p><font size=-1 color=\"#cc0000\">%s</font><p>\n"
                "</center>\n"
                "<p>\n", getWebURL(), getScorecardFile(), getScorecardTitle(), getScorecardTitle(), time_stamp);

        if(getShowMarket())
        {
            fprintf(score, "<center><p><a href = \"%s%s\">Market Value</a></p></center>\n", getWebURL(), getMarketValueFile());
        }
        if(total_players && contest_end)
        {
            int tie = 0;
            String* winner = initString();
            Player* player = getPlayerFromSerialNo(rankings[total_players-1]);
            stringAppend(winner, player->name);
            for(int i=total_players-1; i>0; i--)
            {
                if(wealth[i] == wealth[i-1])
                {
                    player = getPlayerFromSerialNo(rankings[i-1]);
                    stringAppend(winner, CONNECTOR_STR);
                    stringAppend(winner, player->name);
                    tie++;
                }
                else
                {
                    break;
                }
            }
            fprintf(score, "<font color=\"#0000BB\"><b><center><p>Contest Ended</p></center></b></font>\n");
            if(tie)
            {
                fprintf(score, "<font color=\"#0000BB\"><b><center><p>Players %s have tied with %d wealth</p></center></b></font>\n", stringGetBuffer(winner), (int)wealth[total_players-1]);
            }
            else
            {
                fprintf(score, "<font color=\"#0000BB\"><b><center><p>Player %s wins with %d wealth</p></center></b></font>\n", stringGetBuffer(winner), (int)wealth[total_players-1]);
            }
            deinitString(winner);
        }
        fprintf(score,
                "<table align=\"center\">\n"
                "<tr>\n"
                "<th colspan = \"7\"><b><u>Player</u></b></th>\n"
                "<th colspan = \"17\"><b><u>Resources</u></b></th>\n"
                "<th colspan = \"5\"><b><u>Trades</u></b></th>\n"
                "<th colspan = \"9\"><b><u>Wars</u></b></th>\n"
                "<th colspan = \"6\"><b><u>Truces</u></b></th>\n"
                "</tr>"
                "<tr>"
                "<th><u>Name</u></th>\n"
                "<th>&nbsp</th>\n"
                "<th><u>Rank</u></th>\n"
                "<th>&nbsp</th>\n"
                "<th><u>Points</u></th>\n"
                "<th>&nbsp</th>\n"
                "<th>&nbsp</th>\n"
                "<th>&nbsp</th>\n"
                "<th>&nbsp</th>\n"
                "<th><u>%s</u></th>\n"
                "<th>&nbsp</th>\n"
                "<th><u>%s</u></th>\n"
                "<th>&nbsp</th>\n"
                "<th><u>%s</u></th>\n"
                "<th>&nbsp</th>\n"
                "<th><u>%s</u></th>\n"
                "<th>&nbsp</th>\n"
                "<th><u>%s</u></th>\n"
                "<th>&nbsp</th>\n"
                "<th><u>%s</u></th>\n"
                "<th>&nbsp</th>\n"
                "<th><u>%s</u></th>\n"
                "<th>&nbsp</th>\n"
                "<th>&nbsp</th>\n"
                "<th>&nbsp</th>\n"
                "<th>&nbsp</th>\n"
                "<th><u>Performed</u></th>\n"
                "<th>&nbsp</th>\n"
                "<th>&nbsp</th>\n"
                "<th>&nbsp</th>\n"
                "<th>&nbsp</th>\n"
                "<th><u>Fought</u></th>\n"
                "<th>&nbsp</th>\n"
                "<th><u>Initialized</u></th>\n"
                "<th>&nbsp</th>\n"
                "<th><u>Won</u></th>\n"
                "<th>&nbsp</th>\n"
                "<th>&nbsp</th>\n"
                "<th>&nbsp</th>\n"
                "<th>&nbsp</th>\n"
                "<th><u>Offered</u></th>\n"
                "<th>&nbsp</th>\n"
                "<th><u>Accepted</u></th>\n"
                "</tr>\n", R_RUPYULARS_L, R_OIL_L, R_STEEL_L, R_PLASTIC_L, R_COPPER_L, R_GLASS_L, R_RUBBER_L);
    
        for(int i=total_players-1; i>=0; i--)
        {
            Player* player = getPlayerFromSerialNo(rankings[i]);
            fprintf(score, "<tr style=\"text-align:center\"><td>%s</td><th>&nbsp</th><td>%d</td><th>&nbsp</th><td>%d</td><th>&nbsp</th><th>&nbsp</th><th>&nbsp</th><th>&nbsp</th>", player->name, total_players-i, (int)wealth[i]);
            ResourceWallet* player_wallet = getPlayerWallet(player);
            pthread_rwlock_rdlock(player_wallet->lock);
            for(int j=0; j<ASSEMBLED_RESOURCES_START; j++)
            {
                fprintf(score, "<td>%ld</td><th>&nbsp</th>", player_wallet->holdings[j].amount_available);
            }
            pthread_rwlock_unlock(player_wallet->lock);
            fprintf(score, "<th>&nbsp</th><th>&nbsp</th><th>&nbsp</th><td>%d</td><th>&nbsp</th><th>&nbsp</th><th>&nbsp</th><th>&nbsp</th><td>%d</td><th>&nbsp</th><td>%d</td><th>&nbsp</th><td>%d</td><th>&nbsp</th><th>&nbsp</th><th>&nbsp</th><th>&nbsp</th><td>%d</td><th>&nbsp</th><td>%d</td></tr>\n", getPlayerTradesDone(player), getPlayerWarsFought(player), getPlayerWarsInitialized(player), getPlayerWarsWon(player), getPlayerTruceOffered(player), getPlayerTruceAccepted(player));
        }
        fprintf(score,
                "</table>\n"
                "</body>\n"
                "</html>\n");
        fclose(score);
    }
}

void initializeScoreboard()
{
    updateScorecard();
}
