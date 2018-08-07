/*
Copyright (C) 2018 Jonathon Ogden <jeog.dev@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses.
*/

#include <vector>
#include <unordered_map>
#include <iostream>
#include <tuple>
#include <cctype>
#include <string>

#include "../../include/_tdma_api.h"

using namespace std;

namespace tdma {

class MoversGetterImpl
        : public APIGetterImpl{
    MoversIndex _index;
    MoversDirectionType _direction_type;
    MoversChangeType _change_type;

    void
    _build()
    {
        vector<pair<string,string>> params = {{"change", to_string(_change_type)}};

        if( _direction_type != MoversDirectionType::up_and_down ){
            params.emplace_back("direction", to_string(_direction_type));
        }

        string qstr = util::build_encoded_query_str(params);
        string url = URL_MARKETDATA + util::url_encode(to_string(_index))
                     + "/movers?" + qstr;
        APIGetterImpl::set_url(url);
    }

    virtual void
    build()
    { _build(); }

public:
    typedef MoversGetter ProxyType;

    MoversGetterImpl( Credentials& creds,
                        MoversIndex index,
                        MoversDirectionType direction_type,
                        MoversChangeType change_type )
        :
            APIGetterImpl(creds, data_api_on_error_callback),
            _index(index),
            _direction_type(direction_type),
            _change_type(change_type)
        {
            _build();
        }

    MoversIndex
    get_index() const
    { return _index; }

    MoversDirectionType
    get_direction_type() const
    { return _direction_type; }

    MoversChangeType
    get_change_type() const
    { return _change_type; }

    void
    set_index(MoversIndex index)
    {
        _index = index;
        build();
    }

    void
    set_direction_type(MoversDirectionType direction_type)
    {
        _direction_type = direction_type;
        build();
    }

    void
    set_change_type(MoversChangeType change_type)
    {
        _change_type = change_type;
        build();
    }
};

} /* tdma */

using namespace tdma;

int
MoversGetter_Create_ABI( struct Credentials *pcreds,
                            int index,
                            int direction_type,
                            int change_type,
                            MoversGetter_C *pgetter,
                            int allow_exceptions )
{
    int err = getter_is_creatable<MoversGetterImpl>(
        pcreds, pgetter, allow_exceptions
        );
    if( err )
        return err;

    err = check_abi_enum(MoversIndex_is_valid, index, pgetter, allow_exceptions);
    if( err )
        return err;

    err = check_abi_enum(MoversDirectionType_is_valid, direction_type,
                         pgetter, allow_exceptions);
    if( err )
        return err;

    err = check_abi_enum(MoversChangeType_is_valid, change_type, pgetter,
                         allow_exceptions);
    if( err )
        return err;

    static auto meth = +[](Credentials *c, int m, int d, int ct){
        return new MoversGetterImpl(*c, static_cast<MoversIndex>(m),
                                    static_cast<MoversDirectionType>(d),
                                    static_cast<MoversChangeType>(ct));
    };

    MoversGetterImpl *obj;
    tie(obj, err) = CallImplFromABI( allow_exceptions, meth, pcreds,
                                     index, direction_type, change_type);
    if( err ){
        pgetter->obj = nullptr;
        pgetter->type_id = -1;
        return err;
    }

    pgetter->obj = reinterpret_cast<void*>(obj);
    assert( MoversGetter::TYPE_ID_LOW == MoversGetter::TYPE_ID_HIGH );
    pgetter->type_id = MoversGetter::TYPE_ID_LOW;
    return 0;
}

int
MoversGetter_Destroy_ABI( MoversGetter_C *pgetter, int allow_exceptions)
{
    return destroy_getter<MoversGetterImpl>(pgetter, allow_exceptions);
}

int
MoversGetter_GetIndex_ABI( MoversGetter_C *pgetter,
                              int *index,
                              int allow_exceptions)
{
    return GetterImplAccessor<int>::template get<MoversGetterImpl, MoversIndex>(
        pgetter, &MoversGetterImpl::get_index, index, "index", allow_exceptions
        );
}

int
MoversGetter_SetIndex_ABI( MoversGetter_C *pgetter,
                              int index,
                              int allow_exceptions )
{
    int err = check_abi_enum(MoversIndex_is_valid, index, allow_exceptions);
    if( err )
        return err;

    return GetterImplAccessor<int>::template set<MoversGetterImpl, MoversIndex>(
        pgetter, &MoversGetterImpl::set_index, index, allow_exceptions
        );
}

int
MoversGetter_GetDirectionType_ABI( MoversGetter_C *pgetter,
                                       int *direction_type,
                                       int allow_exceptions)
{
    return GetterImplAccessor<int>::template
        get<MoversGetterImpl, MoversDirectionType>(
            pgetter, &MoversGetterImpl::get_direction_type, direction_type,
            "direction_type", allow_exceptions
        );
}

int
MoversGetter_SetDirectionType_ABI( MoversGetter_C *pgetter,
                                       int direction_type,
                                       int allow_exceptions )
{
    int err = check_abi_enum(MoversDirectionType_is_valid, direction_type,
                             allow_exceptions);
    if( err )
        return err;

    return GetterImplAccessor<int>::template
        set<MoversGetterImpl, MoversDirectionType>(
            pgetter, &MoversGetterImpl::set_direction_type, direction_type,
            allow_exceptions
        );
}

int
MoversGetter_GetChangeType_ABI( MoversGetter_C *pgetter,
                                   int *change_type,
                                   int allow_exceptions)
{
    return GetterImplAccessor<int>::template
        get<MoversGetterImpl, MoversChangeType>(
            pgetter, &MoversGetterImpl::get_change_type, change_type,
            "change_type", allow_exceptions
        );
}

int
MoversGetter_SetChangeType_ABI( MoversGetter_C *pgetter,
                                   int change_type,
                                   int allow_exceptions )
{
    int err = check_abi_enum(MoversChangeType_is_valid, change_type,
                             allow_exceptions);
    if( err )
        return err;

    return GetterImplAccessor<int>::template
        set<MoversGetterImpl, MoversChangeType>(
            pgetter, &MoversGetterImpl::set_change_type, change_type,
            allow_exceptions
        );
}




