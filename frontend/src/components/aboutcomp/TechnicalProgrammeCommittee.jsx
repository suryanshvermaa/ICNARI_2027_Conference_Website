import { useEffect, useState } from "react";
import axios from "axios";
import {useCookies} from "react-cookie";

export const TechnicalProgrammeCommitteeLoader = () => {
  const cards = [1, 2, 3, 4];
  return (
    <div className="min-h-screen bg-zinc-50 dark:bg-slate-950 flex justify-center items-center py-12 px-4 sm:px-6 lg:px-8">
      <section className="site-card w-full max-w-7xl p-8 md:p-12">
        <div className="h-10 bg-zinc-200 dark:bg-slate-800/60 rounded-lg w-64 mx-auto mb-8 animate-pulse"></div>
        <div className="grid grid-cols-1 sm:grid-cols-2 md:grid-cols-3 lg:grid-cols-4 gap-8">
          {cards.map((item) => (
            <div
              key={item}
              className="rounded-2xl border border-zinc-200 bg-white p-6 shadow-sm flex flex-col items-center animate-pulse dark:border-slate-700/60 dark:bg-slate-900/40"
              role="region"
              aria-label="Loading committee member"
            >
              <div className="w-28 h-28 rounded-full bg-zinc-200 dark:bg-slate-800/60 mb-4"></div>
              <div className="h-5 bg-zinc-200 dark:bg-slate-800/60 rounded w-3/4 mb-2"></div>
              <div className="h-4 bg-zinc-200 dark:bg-slate-800/60 rounded w-1/2"></div>
            </div>
          ))}
        </div>
      </section>
    </div>
  );
};

/* --- Main Card Improvements --- */
const TechnicalProgrammeCommittee = () => {
  const [loading, setLoading] = useState(false);
  const [committeeMembers, setCommitteeMembers] = useState(null);
  const [cookies, setCookie] = useCookies(['tpcMembersCache']);
  useEffect(() => {
    async function fetchAllCommitteeMembers() {
      setLoading(true);
      try {
        if(cookies.tpcMembersCache){
          setCommitteeMembers(cookies.tpcMembersCache);
          setLoading(false);
          return;
        }
        const res = await axios.get(
          `${import.meta.env.VITE_API_URL}/technicalcommitee/getAllMembers`
        );
        setCookie("tpcMembersCache", res.data.members, {
          path: "/",
          maxAge: 86400,
        }); // Cache for 1 day
        setCommitteeMembers(res.data.members);
        setLoading(false);
      } catch {
        setLoading(false);
        alert("Failed to load techical programme committee members data");
      }
    }
    fetchAllCommitteeMembers();
  }, []);
  if (loading) {
    return <TechnicalProgrammeCommitteeLoader />;
  } else {
    return (
      <div className="min-h-screen bg-zinc-50 dark:bg-slate-950 flex justify-center items-center py-12 px-4 sm:px-6 lg:px-8">
        <section className="site-card w-full max-w-7xl p-8 md:p-12">
          <h2 className="text-3xl md:text-4xl font-bold text-zinc-900 dark:text-slate-50 mb-10 text-center underline underline-offset-8 decoration-blue-400">
            Technical Programme Committee
          </h2>
          <div className="grid grid-cols-1 sm:grid-cols-2 md:grid-cols-3 lg:grid-cols-4 gap-8">
            {committeeMembers &&
              committeeMembers.map(
                (member, idx) =>
                  member?.name && (
                    <div
                      key={idx}
                      className="rounded-2xl border border-zinc-200 bg-white p-6 shadow-sm flex flex-col items-center hover:shadow-md transition-shadow duration-300 dark:border-slate-700/60 dark:bg-slate-900/40"
                      role="region"
                      aria-label={`Committee member: ${member.name}`}
                    >
                      <img
                        src={member.imageUrl}
                        alt={member.name}
                        className="w-28 h-28 rounded-full mb-4 border-4 border-blue-100 dark:border-blue-500/20 object-cover shadow"
                      />
                      <h4 className="text-lg font-semibold text-zinc-900 dark:text-slate-50 text-center">
                        {member.name}
                      </h4>
                      <p className="text-sm text-zinc-600 dark:text-slate-300 text-center mt-1">
                        {member.college}
                      </p>
                      <p className="text-xs text-zinc-500 dark:text-slate-300 text-center mt-2 italic">
                        {Array.isArray(member.specialization)
                          ? member.specialization.join(", ")
                          : member.specialization}
                      </p>
                      {member.role && (
                        <span className="mt-3 inline-block px-3 py-1 bg-blue-50 text-blue-700 dark:bg-blue-500/15 dark:text-blue-200 text-xs rounded-full font-medium">
                          {member.role}
                        </span>
                      )}
                    </div>
                  )
              )}
          </div>
        </section>
      </div>
    );
  }
};

export default TechnicalProgrammeCommittee;
