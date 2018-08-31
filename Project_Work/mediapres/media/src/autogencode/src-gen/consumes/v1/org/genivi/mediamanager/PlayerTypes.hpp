/*
* This file was generated by the CommonAPI Generators.
* Used org.genivi.commonapi.core .
* Used org.franca.core 0.9.1.201412191134.
*
* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
* If a copy of the MPL was not distributed with this file, You can obtain one at
* http://mozilla.org/MPL/2.0/.
*/
#ifndef V1_ORG_GENIVI_MEDIAMANAGER_Player_Types_HPP_
#define V1_ORG_GENIVI_MEDIAMANAGER_Player_Types_HPP_



#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#endif

#include <CommonAPI/Deployment.hpp>
#include <CommonAPI/InputStream.hpp>
#include <CommonAPI/OutputStream.hpp>
#include <CommonAPI/Struct.hpp>
#include <CommonAPI/Types.hpp>
#include <cstdint>

#undef COMMONAPI_INTERNAL_COMPILATION

namespace v1 {
namespace org {
namespace genivi {
namespace mediamanager {

struct PlayerTypes {
    
    struct MuteStatus : CommonAPI::Enumeration<int32_t> {
        enum Literal : int32_t {
            MUTED = 0,
            UNMUTED = 1
        };
        
        MuteStatus() 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(Literal::MUTED)) {}
        MuteStatus(Literal _literal) 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(_literal)) {}
    
        inline bool validate() const {
            switch (value_) {
                case static_cast<int32_t>(Literal::MUTED):
                case static_cast<int32_t>(Literal::UNMUTED):
                return true;
            default:
                return false;
            }
        }
    
        inline bool operator==(const MuteStatus &_other) const { return (value_ == _other.value_); }
        inline bool operator!=(const MuteStatus &_other) const { return (value_ != _other.value_); }
        inline bool operator<=(const MuteStatus &_other) const { return (value_ <= _other.value_); }
        inline bool operator>=(const MuteStatus &_other) const { return (value_ >= _other.value_); }
        inline bool operator<(const MuteStatus &_other) const { return (value_ < _other.value_); }
        inline bool operator>(const MuteStatus &_other) const { return (value_ > _other.value_); }
        
        inline bool operator==(const Literal &_value) const { return (value_ == static_cast<int32_t>(_value)); }
        inline bool operator!=(const Literal &_value) const { return (value_ != static_cast<int32_t>(_value)); }
        inline bool operator<=(const Literal &_value) const { return (value_ <= static_cast<int32_t>(_value)); }
        inline bool operator>=(const Literal &_value) const { return (value_ >= static_cast<int32_t>(_value)); }
        inline bool operator<(const Literal &_value) const { return (value_ < static_cast<int32_t>(_value)); }
        inline bool operator>(const Literal &_value) const { return (value_ > static_cast<int32_t>(_value)); }
    };
    
    struct PlayerError : CommonAPI::Enumeration<int32_t> {
        enum Literal : int32_t {
            NO_ERROR = 0,
            BAD_PLAYLIST = 1,
            BACKEND_UNREACHABLE = 2,
            ERROR_DRM = 3,
            ERROR_COMMUNICATION = 4,
            ERROR_FILEUNAVAILABLE = 5,
            ERROR_FILENOTEXIST = 6,
            ERROR_FILECRRUPT = 7,
            ERROR_PERMISSION = 8,
            ERROR_NOTSUPPORT = 9,
            INVALID_AUDIOLANGUAGE = 10,
            INVALID_SUBTITLE = 11,
            UNAVAILABLE_SUBTITLE = 12,
            UNAVAILABLE_VIDEO = 13,
            EMPTY_PLAYLIST = 14,
            UNAVAILABLE_PLAYLIST = 15,
            UNKNOWN_STREAM_AUDIO = 16,
            ERROR_DEVICE_STATE = 17,
            ERROR_FOLDERNOTEXIST = 18
        };
        
        PlayerError() 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(Literal::NO_ERROR)) {}
        PlayerError(Literal _literal) 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(_literal)) {}
    
        inline bool validate() const {
            switch (value_) {
                case static_cast<int32_t>(Literal::NO_ERROR):
                case static_cast<int32_t>(Literal::BAD_PLAYLIST):
                case static_cast<int32_t>(Literal::BACKEND_UNREACHABLE):
                case static_cast<int32_t>(Literal::ERROR_DRM):
                case static_cast<int32_t>(Literal::ERROR_COMMUNICATION):
                case static_cast<int32_t>(Literal::ERROR_FILEUNAVAILABLE):
                case static_cast<int32_t>(Literal::ERROR_FILENOTEXIST):
                case static_cast<int32_t>(Literal::ERROR_FILECRRUPT):
                case static_cast<int32_t>(Literal::ERROR_PERMISSION):
                case static_cast<int32_t>(Literal::ERROR_NOTSUPPORT):
                case static_cast<int32_t>(Literal::INVALID_AUDIOLANGUAGE):
                case static_cast<int32_t>(Literal::INVALID_SUBTITLE):
                case static_cast<int32_t>(Literal::UNAVAILABLE_SUBTITLE):
                case static_cast<int32_t>(Literal::UNAVAILABLE_VIDEO):
                case static_cast<int32_t>(Literal::EMPTY_PLAYLIST):
                case static_cast<int32_t>(Literal::UNAVAILABLE_PLAYLIST):
                case static_cast<int32_t>(Literal::UNKNOWN_STREAM_AUDIO):
                case static_cast<int32_t>(Literal::ERROR_DEVICE_STATE):
                case static_cast<int32_t>(Literal::ERROR_FOLDERNOTEXIST):
                return true;
            default:
                return false;
            }
        }
    
        inline bool operator==(const PlayerError &_other) const { return (value_ == _other.value_); }
        inline bool operator!=(const PlayerError &_other) const { return (value_ != _other.value_); }
        inline bool operator<=(const PlayerError &_other) const { return (value_ <= _other.value_); }
        inline bool operator>=(const PlayerError &_other) const { return (value_ >= _other.value_); }
        inline bool operator<(const PlayerError &_other) const { return (value_ < _other.value_); }
        inline bool operator>(const PlayerError &_other) const { return (value_ > _other.value_); }
        
        inline bool operator==(const Literal &_value) const { return (value_ == static_cast<int32_t>(_value)); }
        inline bool operator!=(const Literal &_value) const { return (value_ != static_cast<int32_t>(_value)); }
        inline bool operator<=(const Literal &_value) const { return (value_ <= static_cast<int32_t>(_value)); }
        inline bool operator>=(const Literal &_value) const { return (value_ >= static_cast<int32_t>(_value)); }
        inline bool operator<(const Literal &_value) const { return (value_ < static_cast<int32_t>(_value)); }
        inline bool operator>(const Literal &_value) const { return (value_ > static_cast<int32_t>(_value)); }
    };
    
    struct PlayerType : CommonAPI::Enumeration<int32_t> {
        enum Literal : int32_t {
            AUDIO = 0,
            VIDEO = 1,
            IMAGE = 2,
            SIRI = 3
        };
        
        PlayerType() 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(Literal::AUDIO)) {}
        PlayerType(Literal _literal) 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(_literal)) {}
    
        inline bool validate() const {
            switch (value_) {
                case static_cast<int32_t>(Literal::AUDIO):
                case static_cast<int32_t>(Literal::VIDEO):
                case static_cast<int32_t>(Literal::IMAGE):
                case static_cast<int32_t>(Literal::SIRI):
                return true;
            default:
                return false;
            }
        }
    
        inline bool operator==(const PlayerType &_other) const { return (value_ == _other.value_); }
        inline bool operator!=(const PlayerType &_other) const { return (value_ != _other.value_); }
        inline bool operator<=(const PlayerType &_other) const { return (value_ <= _other.value_); }
        inline bool operator>=(const PlayerType &_other) const { return (value_ >= _other.value_); }
        inline bool operator<(const PlayerType &_other) const { return (value_ < _other.value_); }
        inline bool operator>(const PlayerType &_other) const { return (value_ > _other.value_); }
        
        inline bool operator==(const Literal &_value) const { return (value_ == static_cast<int32_t>(_value)); }
        inline bool operator!=(const Literal &_value) const { return (value_ != static_cast<int32_t>(_value)); }
        inline bool operator<=(const Literal &_value) const { return (value_ <= static_cast<int32_t>(_value)); }
        inline bool operator>=(const Literal &_value) const { return (value_ >= static_cast<int32_t>(_value)); }
        inline bool operator<(const Literal &_value) const { return (value_ < static_cast<int32_t>(_value)); }
        inline bool operator>(const Literal &_value) const { return (value_ > static_cast<int32_t>(_value)); }
    };
    
    struct ShuffleStatus : CommonAPI::Enumeration<int32_t> {
        enum Literal : int32_t {
            SHUFFLE = 0,
            UNSHUFFLE = 1
        };
        
        ShuffleStatus() 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(Literal::SHUFFLE)) {}
        ShuffleStatus(Literal _literal) 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(_literal)) {}
    
        inline bool validate() const {
            switch (value_) {
                case static_cast<int32_t>(Literal::SHUFFLE):
                case static_cast<int32_t>(Literal::UNSHUFFLE):
                return true;
            default:
                return false;
            }
        }
    
        inline bool operator==(const ShuffleStatus &_other) const { return (value_ == _other.value_); }
        inline bool operator!=(const ShuffleStatus &_other) const { return (value_ != _other.value_); }
        inline bool operator<=(const ShuffleStatus &_other) const { return (value_ <= _other.value_); }
        inline bool operator>=(const ShuffleStatus &_other) const { return (value_ >= _other.value_); }
        inline bool operator<(const ShuffleStatus &_other) const { return (value_ < _other.value_); }
        inline bool operator>(const ShuffleStatus &_other) const { return (value_ > _other.value_); }
        
        inline bool operator==(const Literal &_value) const { return (value_ == static_cast<int32_t>(_value)); }
        inline bool operator!=(const Literal &_value) const { return (value_ != static_cast<int32_t>(_value)); }
        inline bool operator<=(const Literal &_value) const { return (value_ <= static_cast<int32_t>(_value)); }
        inline bool operator>=(const Literal &_value) const { return (value_ >= static_cast<int32_t>(_value)); }
        inline bool operator<(const Literal &_value) const { return (value_ < static_cast<int32_t>(_value)); }
        inline bool operator>(const Literal &_value) const { return (value_ > static_cast<int32_t>(_value)); }
    };
    
    struct RepeatStatus : CommonAPI::Enumeration<int32_t> {
        enum Literal : int32_t {
            REPEAT = 0,
            REPEAT_SINGLE = 1,
            NO_REPEAT = 2,
            REPEAT_CHANGE = 3
        };
        
        RepeatStatus() 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(Literal::REPEAT)) {}
        RepeatStatus(Literal _literal) 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(_literal)) {}
    
        inline bool validate() const {
            switch (value_) {
                case static_cast<int32_t>(Literal::REPEAT):
                case static_cast<int32_t>(Literal::REPEAT_SINGLE):
                case static_cast<int32_t>(Literal::NO_REPEAT):
                case static_cast<int32_t>(Literal::REPEAT_CHANGE):
                return true;
            default:
                return false;
            }
        }
    
        inline bool operator==(const RepeatStatus &_other) const { return (value_ == _other.value_); }
        inline bool operator!=(const RepeatStatus &_other) const { return (value_ != _other.value_); }
        inline bool operator<=(const RepeatStatus &_other) const { return (value_ <= _other.value_); }
        inline bool operator>=(const RepeatStatus &_other) const { return (value_ >= _other.value_); }
        inline bool operator<(const RepeatStatus &_other) const { return (value_ < _other.value_); }
        inline bool operator>(const RepeatStatus &_other) const { return (value_ > _other.value_); }
        
        inline bool operator==(const Literal &_value) const { return (value_ == static_cast<int32_t>(_value)); }
        inline bool operator!=(const Literal &_value) const { return (value_ != static_cast<int32_t>(_value)); }
        inline bool operator<=(const Literal &_value) const { return (value_ <= static_cast<int32_t>(_value)); }
        inline bool operator>=(const Literal &_value) const { return (value_ >= static_cast<int32_t>(_value)); }
        inline bool operator<(const Literal &_value) const { return (value_ < static_cast<int32_t>(_value)); }
        inline bool operator>(const Literal &_value) const { return (value_ > static_cast<int32_t>(_value)); }
    };
    
    struct ScanStatus : CommonAPI::Enumeration<int32_t> {
        enum Literal : int32_t {
            On = 0,
            Off = 1
        };
        
        ScanStatus() 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(Literal::On)) {}
        ScanStatus(Literal _literal) 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(_literal)) {}
    
        inline bool validate() const {
            switch (value_) {
                case static_cast<int32_t>(Literal::On):
                case static_cast<int32_t>(Literal::Off):
                return true;
            default:
                return false;
            }
        }
    
        inline bool operator==(const ScanStatus &_other) const { return (value_ == _other.value_); }
        inline bool operator!=(const ScanStatus &_other) const { return (value_ != _other.value_); }
        inline bool operator<=(const ScanStatus &_other) const { return (value_ <= _other.value_); }
        inline bool operator>=(const ScanStatus &_other) const { return (value_ >= _other.value_); }
        inline bool operator<(const ScanStatus &_other) const { return (value_ < _other.value_); }
        inline bool operator>(const ScanStatus &_other) const { return (value_ > _other.value_); }
        
        inline bool operator==(const Literal &_value) const { return (value_ == static_cast<int32_t>(_value)); }
        inline bool operator!=(const Literal &_value) const { return (value_ != static_cast<int32_t>(_value)); }
        inline bool operator<=(const Literal &_value) const { return (value_ <= static_cast<int32_t>(_value)); }
        inline bool operator>=(const Literal &_value) const { return (value_ >= static_cast<int32_t>(_value)); }
        inline bool operator<(const Literal &_value) const { return (value_ < static_cast<int32_t>(_value)); }
        inline bool operator>(const Literal &_value) const { return (value_ > static_cast<int32_t>(_value)); }
    };
    
    struct PlaybackStatus : CommonAPI::Enumeration<int32_t> {
        enum Literal : int32_t {
            PLAYING = 0,
            PAUSED = 1,
            STOPPED = 2
        };
        
        PlaybackStatus() 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(Literal::PLAYING)) {}
        PlaybackStatus(Literal _literal) 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(_literal)) {}
    
        inline bool validate() const {
            switch (value_) {
                case static_cast<int32_t>(Literal::PLAYING):
                case static_cast<int32_t>(Literal::PAUSED):
                case static_cast<int32_t>(Literal::STOPPED):
                return true;
            default:
                return false;
            }
        }
    
        inline bool operator==(const PlaybackStatus &_other) const { return (value_ == _other.value_); }
        inline bool operator!=(const PlaybackStatus &_other) const { return (value_ != _other.value_); }
        inline bool operator<=(const PlaybackStatus &_other) const { return (value_ <= _other.value_); }
        inline bool operator>=(const PlaybackStatus &_other) const { return (value_ >= _other.value_); }
        inline bool operator<(const PlaybackStatus &_other) const { return (value_ < _other.value_); }
        inline bool operator>(const PlaybackStatus &_other) const { return (value_ > _other.value_); }
        
        inline bool operator==(const Literal &_value) const { return (value_ == static_cast<int32_t>(_value)); }
        inline bool operator!=(const Literal &_value) const { return (value_ != static_cast<int32_t>(_value)); }
        inline bool operator<=(const Literal &_value) const { return (value_ <= static_cast<int32_t>(_value)); }
        inline bool operator>=(const Literal &_value) const { return (value_ >= static_cast<int32_t>(_value)); }
        inline bool operator<(const Literal &_value) const { return (value_ < static_cast<int32_t>(_value)); }
        inline bool operator>(const Literal &_value) const { return (value_ > static_cast<int32_t>(_value)); }
    };
    
    struct AudioChannelStatus : CommonAPI::Enumeration<int32_t> {
        enum Literal : int32_t {
            STEREO = 0,
            LEFT = 1,
            RIGHT = 2
        };
        
        AudioChannelStatus() 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(Literal::STEREO)) {}
        AudioChannelStatus(Literal _literal) 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(_literal)) {}
    
        inline bool validate() const {
            switch (value_) {
                case static_cast<int32_t>(Literal::STEREO):
                case static_cast<int32_t>(Literal::LEFT):
                case static_cast<int32_t>(Literal::RIGHT):
                return true;
            default:
                return false;
            }
        }
    
        inline bool operator==(const AudioChannelStatus &_other) const { return (value_ == _other.value_); }
        inline bool operator!=(const AudioChannelStatus &_other) const { return (value_ != _other.value_); }
        inline bool operator<=(const AudioChannelStatus &_other) const { return (value_ <= _other.value_); }
        inline bool operator>=(const AudioChannelStatus &_other) const { return (value_ >= _other.value_); }
        inline bool operator<(const AudioChannelStatus &_other) const { return (value_ < _other.value_); }
        inline bool operator>(const AudioChannelStatus &_other) const { return (value_ > _other.value_); }
        
        inline bool operator==(const Literal &_value) const { return (value_ == static_cast<int32_t>(_value)); }
        inline bool operator!=(const Literal &_value) const { return (value_ != static_cast<int32_t>(_value)); }
        inline bool operator<=(const Literal &_value) const { return (value_ <= static_cast<int32_t>(_value)); }
        inline bool operator>=(const Literal &_value) const { return (value_ >= static_cast<int32_t>(_value)); }
        inline bool operator<(const Literal &_value) const { return (value_ < static_cast<int32_t>(_value)); }
        inline bool operator>(const Literal &_value) const { return (value_ > static_cast<int32_t>(_value)); }
    };
    
    struct AspectModeStatus : CommonAPI::Enumeration<int32_t> {
        enum Literal : int32_t {
            PRESERVE = 0,
            CROP = 1,
            STRETCH = 2,
            NATIVE = 3
        };
        
        AspectModeStatus() 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(Literal::PRESERVE)) {}
        AspectModeStatus(Literal _literal) 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(_literal)) {}
    
        inline bool validate() const {
            switch (value_) {
                case static_cast<int32_t>(Literal::PRESERVE):
                case static_cast<int32_t>(Literal::CROP):
                case static_cast<int32_t>(Literal::STRETCH):
                case static_cast<int32_t>(Literal::NATIVE):
                return true;
            default:
                return false;
            }
        }
    
        inline bool operator==(const AspectModeStatus &_other) const { return (value_ == _other.value_); }
        inline bool operator!=(const AspectModeStatus &_other) const { return (value_ != _other.value_); }
        inline bool operator<=(const AspectModeStatus &_other) const { return (value_ <= _other.value_); }
        inline bool operator>=(const AspectModeStatus &_other) const { return (value_ >= _other.value_); }
        inline bool operator<(const AspectModeStatus &_other) const { return (value_ < _other.value_); }
        inline bool operator>(const AspectModeStatus &_other) const { return (value_ > _other.value_); }
        
        inline bool operator==(const Literal &_value) const { return (value_ == static_cast<int32_t>(_value)); }
        inline bool operator!=(const Literal &_value) const { return (value_ != static_cast<int32_t>(_value)); }
        inline bool operator<=(const Literal &_value) const { return (value_ <= static_cast<int32_t>(_value)); }
        inline bool operator>=(const Literal &_value) const { return (value_ >= static_cast<int32_t>(_value)); }
        inline bool operator<(const Literal &_value) const { return (value_ < static_cast<int32_t>(_value)); }
        inline bool operator>(const Literal &_value) const { return (value_ > static_cast<int32_t>(_value)); }
    };
    /**
     * description: rectangle coordinates.
     */
    struct tRectangle : CommonAPI::Struct<uint32_t, uint32_t, uint32_t, uint32_t> {
    	
    	tRectangle() {
    	}
    	tRectangle(const uint32_t &_left, const uint32_t &_top, const uint32_t &_right, const uint32_t &_bottom)
    	{
    		std::get<0>(values_) = _left;
    		std::get<1>(values_) = _top;
    		std::get<2>(values_) = _right;
    		std::get<3>(values_) = _bottom;
    	}
    	inline const uint32_t &getLeft() const { return std::get<0>(values_); }
    	inline void setLeft(const uint32_t &_value) { std::get<0>(values_) = _value; }
    	inline const uint32_t &getTop() const { return std::get<1>(values_); }
    	inline void setTop(const uint32_t &_value) { std::get<1>(values_) = _value; }
    	inline const uint32_t &getRight() const { return std::get<2>(values_); }
    	inline void setRight(const uint32_t &_value) { std::get<2>(values_) = _value; }
    	inline const uint32_t &getBottom() const { return std::get<3>(values_); }
    	inline void setBottom(const uint32_t &_value) { std::get<3>(values_) = _value; }
    	inline bool operator==(const tRectangle& _other) const {
                return (getLeft() == _other.getLeft() && getTop() == _other.getTop() && getRight() == _other.getRight() && getBottom() == _other.getBottom());
        }
    	inline bool operator!=(const tRectangle &_other) const {
    		return !((*this) == _other);
    	}
    
    };
    
    struct SubtitleStatus : CommonAPI::Enumeration<int32_t> {
        enum Literal : int32_t {
            ON = 0,
            OFF = 1
        };
        
        SubtitleStatus() 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(Literal::ON)) {}
        SubtitleStatus(Literal _literal) 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(_literal)) {}
    
        inline bool validate() const {
            switch (value_) {
                case static_cast<int32_t>(Literal::ON):
                case static_cast<int32_t>(Literal::OFF):
                return true;
            default:
                return false;
            }
        }
    
        inline bool operator==(const SubtitleStatus &_other) const { return (value_ == _other.value_); }
        inline bool operator!=(const SubtitleStatus &_other) const { return (value_ != _other.value_); }
        inline bool operator<=(const SubtitleStatus &_other) const { return (value_ <= _other.value_); }
        inline bool operator>=(const SubtitleStatus &_other) const { return (value_ >= _other.value_); }
        inline bool operator<(const SubtitleStatus &_other) const { return (value_ < _other.value_); }
        inline bool operator>(const SubtitleStatus &_other) const { return (value_ > _other.value_); }
        
        inline bool operator==(const Literal &_value) const { return (value_ == static_cast<int32_t>(_value)); }
        inline bool operator!=(const Literal &_value) const { return (value_ != static_cast<int32_t>(_value)); }
        inline bool operator<=(const Literal &_value) const { return (value_ <= static_cast<int32_t>(_value)); }
        inline bool operator>=(const Literal &_value) const { return (value_ >= static_cast<int32_t>(_value)); }
        inline bool operator<(const Literal &_value) const { return (value_ < static_cast<int32_t>(_value)); }
        inline bool operator>(const Literal &_value) const { return (value_ > static_cast<int32_t>(_value)); }
    };
    
    struct RestrictedModeStatus : CommonAPI::Enumeration<int32_t> {
        enum Literal : int32_t {
            OFF = 0,
            ON = 1
        };
        
        RestrictedModeStatus() 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(Literal::OFF)) {}
        RestrictedModeStatus(Literal _literal) 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(_literal)) {}
    
        inline bool validate() const {
            switch (value_) {
                case static_cast<int32_t>(Literal::OFF):
                case static_cast<int32_t>(Literal::ON):
                return true;
            default:
                return false;
            }
        }
    
        inline bool operator==(const RestrictedModeStatus &_other) const { return (value_ == _other.value_); }
        inline bool operator!=(const RestrictedModeStatus &_other) const { return (value_ != _other.value_); }
        inline bool operator<=(const RestrictedModeStatus &_other) const { return (value_ <= _other.value_); }
        inline bool operator>=(const RestrictedModeStatus &_other) const { return (value_ >= _other.value_); }
        inline bool operator<(const RestrictedModeStatus &_other) const { return (value_ < _other.value_); }
        inline bool operator>(const RestrictedModeStatus &_other) const { return (value_ > _other.value_); }
        
        inline bool operator==(const Literal &_value) const { return (value_ == static_cast<int32_t>(_value)); }
        inline bool operator!=(const Literal &_value) const { return (value_ != static_cast<int32_t>(_value)); }
        inline bool operator<=(const Literal &_value) const { return (value_ <= static_cast<int32_t>(_value)); }
        inline bool operator>=(const Literal &_value) const { return (value_ >= static_cast<int32_t>(_value)); }
        inline bool operator<(const Literal &_value) const { return (value_ < static_cast<int32_t>(_value)); }
        inline bool operator>(const Literal &_value) const { return (value_ > static_cast<int32_t>(_value)); }
    };
    
    struct StartIndexOption : CommonAPI::Enumeration<int32_t> {
        enum Literal : int32_t {
            HEAD = 0,
            TAIL = 1
        };
        
        StartIndexOption() 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(Literal::HEAD)) {}
        StartIndexOption(Literal _literal) 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(_literal)) {}
    
        inline bool validate() const {
            switch (value_) {
                case static_cast<int32_t>(Literal::HEAD):
                case static_cast<int32_t>(Literal::TAIL):
                return true;
            default:
                return false;
            }
        }
    
        inline bool operator==(const StartIndexOption &_other) const { return (value_ == _other.value_); }
        inline bool operator!=(const StartIndexOption &_other) const { return (value_ != _other.value_); }
        inline bool operator<=(const StartIndexOption &_other) const { return (value_ <= _other.value_); }
        inline bool operator>=(const StartIndexOption &_other) const { return (value_ >= _other.value_); }
        inline bool operator<(const StartIndexOption &_other) const { return (value_ < _other.value_); }
        inline bool operator>(const StartIndexOption &_other) const { return (value_ > _other.value_); }
        
        inline bool operator==(const Literal &_value) const { return (value_ == static_cast<int32_t>(_value)); }
        inline bool operator!=(const Literal &_value) const { return (value_ != static_cast<int32_t>(_value)); }
        inline bool operator<=(const Literal &_value) const { return (value_ <= static_cast<int32_t>(_value)); }
        inline bool operator>=(const Literal &_value) const { return (value_ >= static_cast<int32_t>(_value)); }
        inline bool operator<(const Literal &_value) const { return (value_ < static_cast<int32_t>(_value)); }
        inline bool operator>(const Literal &_value) const { return (value_ > static_cast<int32_t>(_value)); }
    };
    
    struct IAP2ButtonType : CommonAPI::Enumeration<int32_t> {
        enum Literal : int32_t {
            BUTTON_PLAY = 0,
            BUTTON_PAUSE = 1,
            BUTTON_NEXTTRACK = 2,
            BUTTON_PREVTRACK = 3,
            BUTTON_RANDOM = 4,
            BUTTON_REPEAT = 5,
            BUTTON_NORMALSPEED = 6,
            BUTTON_INCREASESPEED = 7,
            BUTTON_DECREASESPEED = 8,
            BUTTON_PLAYPAUSE = 9,
            BUTTON_SIRI = 10,
            BUTTON_MUTE = 11,
            BUTTON_INCREASEVOLUME = 12,
            BUTTON_DECREASEVOLUME = 13,
            BUTTON_PROMOTE = 14,
            BUTTON_DEMOTE = 15,
            BUTTON_ADDTOWISHLIST = 16
        };
        
        IAP2ButtonType() 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(Literal::BUTTON_PLAY)) {}
        IAP2ButtonType(Literal _literal) 
            : CommonAPI::Enumeration<int32_t>(static_cast<int32_t>(_literal)) {}
    
        inline bool validate() const {
            switch (value_) {
                case static_cast<int32_t>(Literal::BUTTON_PLAY):
                case static_cast<int32_t>(Literal::BUTTON_PAUSE):
                case static_cast<int32_t>(Literal::BUTTON_NEXTTRACK):
                case static_cast<int32_t>(Literal::BUTTON_PREVTRACK):
                case static_cast<int32_t>(Literal::BUTTON_RANDOM):
                case static_cast<int32_t>(Literal::BUTTON_REPEAT):
                case static_cast<int32_t>(Literal::BUTTON_NORMALSPEED):
                case static_cast<int32_t>(Literal::BUTTON_INCREASESPEED):
                case static_cast<int32_t>(Literal::BUTTON_DECREASESPEED):
                case static_cast<int32_t>(Literal::BUTTON_PLAYPAUSE):
                case static_cast<int32_t>(Literal::BUTTON_SIRI):
                case static_cast<int32_t>(Literal::BUTTON_MUTE):
                case static_cast<int32_t>(Literal::BUTTON_INCREASEVOLUME):
                case static_cast<int32_t>(Literal::BUTTON_DECREASEVOLUME):
                case static_cast<int32_t>(Literal::BUTTON_PROMOTE):
                case static_cast<int32_t>(Literal::BUTTON_DEMOTE):
                case static_cast<int32_t>(Literal::BUTTON_ADDTOWISHLIST):
                return true;
            default:
                return false;
            }
        }
    
        inline bool operator==(const IAP2ButtonType &_other) const { return (value_ == _other.value_); }
        inline bool operator!=(const IAP2ButtonType &_other) const { return (value_ != _other.value_); }
        inline bool operator<=(const IAP2ButtonType &_other) const { return (value_ <= _other.value_); }
        inline bool operator>=(const IAP2ButtonType &_other) const { return (value_ >= _other.value_); }
        inline bool operator<(const IAP2ButtonType &_other) const { return (value_ < _other.value_); }
        inline bool operator>(const IAP2ButtonType &_other) const { return (value_ > _other.value_); }
        
        inline bool operator==(const Literal &_value) const { return (value_ == static_cast<int32_t>(_value)); }
        inline bool operator!=(const Literal &_value) const { return (value_ != static_cast<int32_t>(_value)); }
        inline bool operator<=(const Literal &_value) const { return (value_ <= static_cast<int32_t>(_value)); }
        inline bool operator>=(const Literal &_value) const { return (value_ >= static_cast<int32_t>(_value)); }
        inline bool operator<(const Literal &_value) const { return (value_ < static_cast<int32_t>(_value)); }
        inline bool operator>(const Literal &_value) const { return (value_ > static_cast<int32_t>(_value)); }
    };



static inline const char* getTypeCollectionName() {
    static const char* typeCollectionName = "org.genivi.mediamanager.PlayerTypes";
    return typeCollectionName;
}

inline CommonAPI::Version getTypeCollectionVersion() {
    return CommonAPI::Version(1, 1);
}

}; // struct PlayerTypes

} // namespace mediamanager
} // namespace genivi
} // namespace org
} // namespace v1

namespace CommonAPI {
}


namespace std {
    //Hash for MuteStatus
    template<>
    struct hash<::v1::org::genivi::mediamanager::PlayerTypes::MuteStatus> {
        inline size_t operator()(const ::v1::org::genivi::mediamanager::PlayerTypes::MuteStatus& muteStatus) const {
            return static_cast<int32_t>(muteStatus);
        }
    };
    //Hash for PlayerError
    template<>
    struct hash<::v1::org::genivi::mediamanager::PlayerTypes::PlayerError> {
        inline size_t operator()(const ::v1::org::genivi::mediamanager::PlayerTypes::PlayerError& playerError) const {
            return static_cast<int32_t>(playerError);
        }
    };
    //Hash for PlayerType
    template<>
    struct hash<::v1::org::genivi::mediamanager::PlayerTypes::PlayerType> {
        inline size_t operator()(const ::v1::org::genivi::mediamanager::PlayerTypes::PlayerType& playerType) const {
            return static_cast<int32_t>(playerType);
        }
    };
    //Hash for ShuffleStatus
    template<>
    struct hash<::v1::org::genivi::mediamanager::PlayerTypes::ShuffleStatus> {
        inline size_t operator()(const ::v1::org::genivi::mediamanager::PlayerTypes::ShuffleStatus& shuffleStatus) const {
            return static_cast<int32_t>(shuffleStatus);
        }
    };
    //Hash for RepeatStatus
    template<>
    struct hash<::v1::org::genivi::mediamanager::PlayerTypes::RepeatStatus> {
        inline size_t operator()(const ::v1::org::genivi::mediamanager::PlayerTypes::RepeatStatus& repeatStatus) const {
            return static_cast<int32_t>(repeatStatus);
        }
    };
    //Hash for ScanStatus
    template<>
    struct hash<::v1::org::genivi::mediamanager::PlayerTypes::ScanStatus> {
        inline size_t operator()(const ::v1::org::genivi::mediamanager::PlayerTypes::ScanStatus& scanStatus) const {
            return static_cast<int32_t>(scanStatus);
        }
    };
    //Hash for PlaybackStatus
    template<>
    struct hash<::v1::org::genivi::mediamanager::PlayerTypes::PlaybackStatus> {
        inline size_t operator()(const ::v1::org::genivi::mediamanager::PlayerTypes::PlaybackStatus& playbackStatus) const {
            return static_cast<int32_t>(playbackStatus);
        }
    };
    //Hash for AudioChannelStatus
    template<>
    struct hash<::v1::org::genivi::mediamanager::PlayerTypes::AudioChannelStatus> {
        inline size_t operator()(const ::v1::org::genivi::mediamanager::PlayerTypes::AudioChannelStatus& audioChannelStatus) const {
            return static_cast<int32_t>(audioChannelStatus);
        }
    };
    //Hash for AspectModeStatus
    template<>
    struct hash<::v1::org::genivi::mediamanager::PlayerTypes::AspectModeStatus> {
        inline size_t operator()(const ::v1::org::genivi::mediamanager::PlayerTypes::AspectModeStatus& aspectModeStatus) const {
            return static_cast<int32_t>(aspectModeStatus);
        }
    };
    //Hash for SubtitleStatus
    template<>
    struct hash<::v1::org::genivi::mediamanager::PlayerTypes::SubtitleStatus> {
        inline size_t operator()(const ::v1::org::genivi::mediamanager::PlayerTypes::SubtitleStatus& subtitleStatus) const {
            return static_cast<int32_t>(subtitleStatus);
        }
    };
    //Hash for RestrictedModeStatus
    template<>
    struct hash<::v1::org::genivi::mediamanager::PlayerTypes::RestrictedModeStatus> {
        inline size_t operator()(const ::v1::org::genivi::mediamanager::PlayerTypes::RestrictedModeStatus& restrictedModeStatus) const {
            return static_cast<int32_t>(restrictedModeStatus);
        }
    };
    //Hash for StartIndexOption
    template<>
    struct hash<::v1::org::genivi::mediamanager::PlayerTypes::StartIndexOption> {
        inline size_t operator()(const ::v1::org::genivi::mediamanager::PlayerTypes::StartIndexOption& startIndexOption) const {
            return static_cast<int32_t>(startIndexOption);
        }
    };
    //Hash for IAP2ButtonType
    template<>
    struct hash<::v1::org::genivi::mediamanager::PlayerTypes::IAP2ButtonType> {
        inline size_t operator()(const ::v1::org::genivi::mediamanager::PlayerTypes::IAP2ButtonType& iAP2ButtonType) const {
            return static_cast<int32_t>(iAP2ButtonType);
        }
    };
}


// Compatibility
namespace v1_1 = v1;

#endif // V1_ORG_GENIVI_MEDIAMANAGER_Player_Types_HPP_
