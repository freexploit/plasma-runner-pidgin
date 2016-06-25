/*
 *   Copyright (C) 2016 Christopher Valerio <christopher@valerio.guru>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef PIDGINRUNNER_H
#define PIDGINRUNNER_H

#include <KRunner/AbstractRunner>
#include "pidgin_dbus.h"


class PidginRunner: public Plasma::AbstractRunner
{

    Q_OBJECT;
    private:
        PidginDbus pidgin_d;

    public:
        PidginRunner(QObject *parent, const QVariantList& args);
        ~PidginRunner();
    
        void match(Plasma::RunnerContext &context);
        void run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match);


};

K_EXPORT_PLASMA_RUNNER(pidgin_runner, PidginRunner)
#endif
