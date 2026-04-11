import React from "react";
import { Link } from "react-router-dom";
import {
  User,
  Mic2,
  BellPlus,
  Users,
  Bell,
  ImagePlus,
  Mail,
  Trash2,
  UsersRound,
  Wrench,
  Globe,
  ClipboardList,
  Factory,
} from "lucide-react";

export default function Admin() {
  const tiles = [
    {
      title: "Profile",
      desc: "Update your account details",
      to: "/admin/profile",
      icon: User,
    },
    {
      title: "Add Speaker",
      desc: "Create a speaker profile",
      to: "/admin/add-speakers",
      icon: Mic2,
    },
    {
      title: "Add Update",
      desc: "Post recent announcements",
      to: "/admin/add-recent-updates",
      icon: BellPlus,
    },
    {
      title: "All Speakers",
      desc: "Manage speakers & priority",
      to: "/admin/all-speakers",
      icon: Users,
    },
    {
      title: "All Updates",
      desc: "Manage announcements",
      to: "/admin/all-updates",
      icon: Bell,
    },
    {
      title: "Upload Photos",
      desc: "Add photos to gallery",
      to: "/admin/photogalleryupload",
      icon: ImagePlus,
    },
    {
      title: "Contact Messages",
      desc: "View contact form messages",
      to: "/admin/contact-messages",
      icon: Mail,
    },
    {
      title: "Delete Photos",
      desc: "Remove photos from gallery",
      to: "/admin/deletephoto",
      icon: Trash2,
    },
    {
      title: "Org Committee",
      desc: "Add or manage members",
      to: "/admin/all-organising-members",
      icon: UsersRound,
    },
    {
      title: "Technical Committee",
      desc: "Add or manage members",
      to: "/admin/all-technical-members",
      icon: Wrench,
    },
    {
      title: "Programme Committee",
      desc: "Add or manage members",
      to: "/admin/all-programme-members",
      icon: ClipboardList,
    },
    {
      title: "Industry Committee",
      desc: "Add or manage members",
      to: "/admin/all-industry-members",
      icon: Factory,
    },
    {
      title: "International Committee",
      desc: "Add or manage members",
      to: "/admin/all-international-members",
      icon: Globe,
    },
  ];

  return (
    <div className="space-y-6">
      <div>
        <h1 className="admin-title">Dashboard</h1>
        <p className="admin-muted">Quick actions for managing the conference website.</p>
      </div>

      <div className="grid gap-4 sm:grid-cols-2 lg:grid-cols-3">
        {tiles.map(({ title, desc, to, icon }) => (
          <Link
            key={to}
            to={to}
            className="admin-card group block overflow-hidden"
          >
            <div className="admin-card-inner">
              <div className="flex items-start justify-between gap-3">
                <div className="flex min-w-0 items-start gap-3">
                  <div className="flex h-10 w-10 items-center justify-center rounded-xl bg-indigo-600/15 ring-1 ring-inset ring-indigo-500/20">
                    {React.createElement(icon, { className: "h-5 w-5 text-indigo-700" })}
                  </div>
                  <div className="min-w-0">
                    <div className="text-sm font-semibold text-zinc-900">{title}</div>
                    <div className="mt-1 text-sm text-zinc-600">{desc}</div>
                  </div>
                </div>
                <div className="text-zinc-400 group-hover:text-zinc-700">→</div>
              </div>
            </div>
          </Link>
        ))}
      </div>
    </div>
  );
}
