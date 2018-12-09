#pragma once

class PlayerStats {
public:
    friend class GameModel;
    friend class MoveMaker;
    int get_accuracy() const
        { return (_hits + _misses == 0) ? 0 : (100 * _hits / (_hits + _misses)); }
    int get_hits() const
        { return _hits; }
    int get_misses() const
         { return _misses; }
    int get_destroyed_ships() const
        { return _shipsDestroyed; }
    bool is_winner() const
        { return _isWinner; }
private:
    void _set_winner()
        { _isWinner = true; }
    void _reset_stats()
        { _hits = _misses = _shipsDestroyed = 0; _isWinner = false; }
    void _add_miss()
        { ++_misses; }
    void _add_hit()
        { ++_hits; }
    void _add_destroyed_ship()
        { ++_shipsDestroyed; }
    // data
    int _hits = 0;
    int _misses = 0;
    int _shipsDestroyed = 0;
    bool _isWinner = false;
};
